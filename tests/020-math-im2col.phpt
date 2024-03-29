--TEST--
Math::im2col
--SKIPIF--
<?php
if (!extension_loaded('rindow_clblast')) {
	echo 'skip';
}
?>
--FILE--
<?php
$loader = include __DIR__.'/../vendor/autoload.php';
include __DIR__.'/../testPHP/HostBuffer.php';
use Interop\Polite\Math\Matrix\OpenCL;
use Interop\Polite\Math\Matrix\BLAS;
use Interop\Polite\Math\Matrix\NDArray;

$context = new Rindow\OpenCL\Context(OpenCL::CL_DEVICE_TYPE_DEFAULT);
$queue = new Rindow\OpenCL\CommandQueue($context);


$kernel_mode = Rindow\CLBlast\Math::CROSS_CORRELATION;
$batches = 1;
$im_h = 5;
$im_w = 5;
$channels = 1;
$kernel_h = 3;
$kernel_w = 3;
$stride_h = 1;
$stride_w = 1;
$padding = true;
$dilation_h = 1;
$dilation_w = 1;
$images_offset = 0;
$col_offset = 0;

$padding_h = 0;
$padding_w = 0;
$out_h = intval(floor(($im_h-($kernel_h-1)*$dilation_h-1)/$stride_h)+1);
$out_w = intval(floor(($im_w-($kernel_w-1)*$dilation_w-1)/$stride_w)+1);

$host_im_buffer  = new RindowTest\CLBlast\HostBuffer($im_h*$im_w*$channels,NDArray::float32);
$host_col_buffer = new RindowTest\CLBlast\HostBuffer($out_h*$out_w*$channels*$kernel_h*$kernel_w,NDArray::float32);
                    
for($i=0;$i<count($host_im_buffer);$i++) {
    $host_im_buffer[$i]=$i;
}
for($i=0;$i<count($host_col_buffer);$i++) {
    $host_col_buffer[$i]=100000;
}
$batch_id = 0;
$trues = [];
for($batch_id=0;$batch_id<$batches;$batch_id++) {
    for($channel_id=0;$channel_id<$channels;$channel_id++) {
        for($kernel_y=0;$kernel_y<$kernel_h;$kernel_y++) {
            for($kernel_x=0;$kernel_x<$kernel_w;$kernel_x++) {
                for($im_y=0;$im_y<$out_h;$im_y++) {
                    for($im_x=0;$im_x<$out_w;$im_x++) {
                        $input_y = $im_y*$stride_h+$kernel_y*$dilation_h-$padding_h;
                        $input_x = $im_x*$stride_w+$kernel_x*$dilation_w-$padding_w;
                        # channel first
                        $input_id = ((($batch_id*$channels+$channel_id)*$im_h+$input_y)*$im_w+$input_x);
                        # channel kernel stride
                        $cols_id = ((((($batch_id*$channels+$channel_id)
                                    *$kernel_h+$kernel_y)*$kernel_w+$kernel_x)*$out_h+$im_y)*$out_w+$im_x);
                        if($input_y>=0 && $input_y<$im_h && $input_x>=0 && $input_x<$im_w) {
                            $trues[$cols_id] = $host_im_buffer[$input_id];
                        }
                    }
                }
            }
        }
    }
}

$im_buffer = new Rindow\OpenCL\Buffer($context,intval(count($host_im_buffer)*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $host_im_buffer);
$col_buffer = new Rindow\OpenCL\Buffer($context,intval(count($host_col_buffer)*32/8),
    OpenCL::CL_MEM_WRITE_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $host_col_buffer);

#CLBlastSim2col(
#    const CLBlastKernelMode kernel_mode,
#    const size_t channels, const size_t height, const size_t width,
#    const size_t kernel_h, const size_t kernel_w,
#    const size_t pad_h, const size_t pad_w,
#    const size_t stride_h, const size_t stride_w,
#    const size_t dilation_h, const size_t dilation_w,
#    const cl_mem im_buffer, const size_t images_offset,
#    cl_mem col_buffer, const size_t col_offset,
#    cl_command_queue* queue, cl_event* event);

$math = new Rindow\CLBlast\Math();
$events = new Rindow\OpenCL\EventList();
$math->im2col($kernel_mode,
    $channels,$im_h,$im_w,
    $kernel_h,$kernel_w,
    $padding_h,$padding_w,
    $stride_h,$stride_w,
    $dilation_h,$dilation_w,
    $im_buffer,$images_offset,
    $col_buffer,$col_offset,
    $queue,$events);
$events->wait();

$col_buffer->read($queue,$host_col_buffer);
#echo "\n=======col======\n";
for($i=0;$i<count($host_col_buffer);$i++) {
    #echo $host_col_buffer[$i].",";
    assert($trues[$i]==$host_col_buffer[$i]);
}
#echo "\n=======im======\n";
for($i=0;$i<count($host_im_buffer);$i++) {
    #echo $host_im_buffer[$i].",";
}
#echo "======\n";
#for($i=0;$i<count($host_col_buffer);$i++) {
#    echo $trues[$i].",";
#}
echo "SUCCESS\n";
//
// invalid object arguments
//
$events = new Rindow\OpenCL\EventList();
$invalidBuffer = new \stdClass();
try {
    $math->im2col($kernel_mode,
        $channels,$im_h,$im_w,
        $kernel_h,$kernel_w,
        $padding_h,$padding_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $invalidBuffer,$images_offset,
        $col_buffer,$col_offset,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $math->im2col($kernel_mode,
        $channels,$im_h,$im_w,
        $kernel_h,$kernel_w,
        $padding_h,$padding_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $im_buffer,$images_offset,
        $invalidBuffer,$col_offset,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidQueue = new \stdClass();
try {
    $math->im2col($kernel_mode,
        $channels,$im_h,$im_w,
        $kernel_h,$kernel_w,
        $padding_h,$padding_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $im_buffer,$images_offset,
        $col_buffer,$col_offset,
        $invalidQueue,$events);
} catch (\Throwable $e) {
    echo "Invalid Queue catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidEvents = new \stdClass();
try {
    $math->im2col($kernel_mode,
        $channels,$im_h,$im_w,
        $kernel_h,$kernel_w,
        $padding_h,$padding_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $im_buffer,$images_offset,
        $col_buffer,$col_offset,
        $queue,$invalidEvents);
} catch (\Throwable $e) {
    echo "Invalid Event catch: ".get_class($e)."\n";
}
echo "SUCCESS invalid object arguments\n";
?>
--EXPECT--
SUCCESS
Invalid Buffer catch: TypeError
Invalid Buffer catch: TypeError
Invalid Queue catch: TypeError
Invalid Event catch: TypeError
SUCCESS invalid object arguments
