--TEST--
Math::col2im
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

$host_col_buffer = new RindowTest\CLBlast\HostBuffer(($height-$kernel_h+1)*($width-$kernel_w+1)*4*$channels,NDArray::float32);
$host_im_buffer  = new RindowTest\CLBlast\HostBuffer($height*$width*$channels,NDArray::float32);
$filter_h = $height-$kernel_h+1;
$filter_w = $width-$kernel_w+1;
$trues = [];
for($i=0;$i<count($host_im_buffer);$i++) {
    $host_im_buffer[$i]=100000;
    $trues[$i]=100000;
}
$i = 0;
for($ch_id=0;$ch_id<$channels;$ch_id++) {
    for($kh_id=$kernel_h-1;$kh_id>=0;$kh_id--) {
        for($kw_id=$kernel_w-1;$kw_id>=0;$kw_id--) {
            for($fh_id=0;$fh_id<$filter_h;$fh_id++) {
                for($fw_id=0;$fw_id<$filter_w;$fw_id++) {
                    $idx = $ch_id*($height*$width)+
                        $kh_id*($width)+
                        $kw_id*(1)+
                        $fh_id*($width)+
                        $fw_id*(1);
                    $host_col_buffer[$i] = $idx;
                    $i++;
                    $trues[$idx] += $idx;
                }
            }
        }
    }
}
$col_buffer = new Rindow\OpenCL\Buffer($context,intval(count($host_col_buffer)*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,$host_col_buffer);
$im_buffer = new Rindow\OpenCL\Buffer($context,intval(count($host_im_buffer)*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,$host_im_buffer,null,NDArray::float32);
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
$math->col2im($kernel_mode,
    $channels,$height,$width,
    $kernel_h,$kernel_w,
    $pad_h,$pad_w,
    $stride_h,$stride_w,
    $dilation_h,$dilation_w,
    $col_buffer,$col_offset,
    $im_buffer,$im_offset,
    $queue,$events);
$events->wait();
$im_buffer->read($queue,$host_im_buffer);
for($i=0;$i<count($host_im_buffer);$i++) {
    assert($trues[$i]==$host_im_buffer[$i]);
}
echo "SUCCESS\n";
?>
--EXPECT--
SUCCESS
