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


$kernel_mode = Rindow\CLBlast\Math::CONVOLUTION;
$channels=3;
$height=4;
$width=4;
$kernel_h = 2;
$kernel_w = 2;
$pad_h = 0;
$pad_w = 0;
$stride_h = 1;
$stride_w = 1;
$dilation_h=1;
$dilation_w=1;
$im_offset =0;
$col_offset =0;
$out_h = $height-$kernel_h+1;
$out_w = $width-$kernel_w+1;

$host_im_buffer  = new RindowTest\CLBlast\HostBuffer($height*$width*$channels,NDArray::float32);
#$host_col_buffer = new RindowTest\CLBlast\HostBuffer(($height-$kernel_h+1)*($width-$kernel_w+1)*4*$channels,NDArray::float32);
$host_col_buffer = new RindowTest\CLBlast\HostBuffer($out_h*$out_w*4*$channels,NDArray::float32);
for($i=0;$i<count($host_im_buffer);$i++) {
    $host_im_buffer[$i]=$i;
}
for($i=0;$i<count($host_col_buffer);$i++) {
    $host_col_buffer[$i]=100000;
}
$trues = [];
$i = 0;
for($ch_id=0;$ch_id<$channels;$ch_id++) {
    for($kh_id=0;$kh_id<$kernel_h;$kh_id++) {
        for($kw_id=0;$kw_id<$kernel_w;$kw_id++) {
            for($fh_id=0;$fh_id<$out_h;$fh_id++) {
                for($fw_id=0;$fw_id<$out_w;$fw_id++) {
                    $image_index =
                        $ch_id*($height*$width)+
                        ($kernel_h-$kh_id-1)*($width)+ ###### CAUTION #####
                        ($kernel_w-$kw_id-1)*(1)+      ###### CAUTION #####
                        $fh_id*($width)+
                        $fw_id*(1);
                    $trues[$i] = $host_im_buffer[$image_index];
                    $i++;
                }
            }
        }
    }
}
$im_buffer = new Rindow\OpenCL\Buffer($context,intval(count($host_im_buffer)*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $host_im_buffer);
$col_buffer = new Rindow\OpenCL\Buffer($context,intval(count($host_col_buffer)*32/8),
    OpenCL::CL_MEM_WRITE_ONLY,null,0,NDArray::float32);

#CLBlastSim2col(
#    const CLBlastKernelMode kernel_mode,
#    const size_t channels, const size_t height, const size_t width,
#    const size_t kernel_h, const size_t kernel_w,
#    const size_t pad_h, const size_t pad_w,
#    const size_t stride_h, const size_t stride_w,
#    const size_t dilation_h, const size_t dilation_w,
#    const cl_mem im_buffer, const size_t im_offset,
#    cl_mem col_buffer, const size_t col_offset,
#    cl_command_queue* queue, cl_event* event);

$math = new Rindow\CLBlast\Math();
$events = new Rindow\OpenCL\EventList();
$math->im2col($kernel_mode,
    $channels,$height,$width,
    $kernel_h,$kernel_w,
    $pad_h,$pad_w,
    $stride_h,$stride_w,
    $dilation_h,$dilation_w,
    $im_buffer,$im_offset,
    $col_buffer,$col_offset,
    $queue,$events);
$events->wait();

$col_buffer->read($queue,$host_col_buffer);
for($i=0;$i<count($host_col_buffer);$i++) {
    #echo $host_col_buffer[$i].",";
    assert($trues[$i]==$host_col_buffer[$i]);
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
        $channels,$height,$width,
        $kernel_h,$kernel_w,
        $pad_h,$pad_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $invalidBuffer,$im_offset,
        $col_buffer,$col_offset,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $math->im2col($kernel_mode,
        $channels,$height,$width,
        $kernel_h,$kernel_w,
        $pad_h,$pad_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $im_buffer,$im_offset,
        $invalidBuffer,$col_offset,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidQueue = new \stdClass();
try {
    $math->im2col($kernel_mode,
        $channels,$height,$width,
        $kernel_h,$kernel_w,
        $pad_h,$pad_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $im_buffer,$im_offset,
        $col_buffer,$col_offset,
        $invalidQueue,$events);
} catch (\Throwable $e) {
    echo "Invalid Queue catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidEvents = new \stdClass();
try {
    $math->im2col($kernel_mode,
        $channels,$height,$width,
        $kernel_h,$kernel_w,
        $pad_h,$pad_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $im_buffer,$im_offset,
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
