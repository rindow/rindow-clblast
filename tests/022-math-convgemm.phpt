--TEST--
Math::convgemm
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
$kernel_h = 3;
$kernel_w = 3;
$pad_h = 0;
$pad_w = 0;
$stride_h = 1;
$stride_w = 1;
$dilation_h=1;
$dilation_w=1;
$num_kernels=5;
$batch_count=6;
$im_offset=0;
$kernel_offset=0;
$result_offset=0;
$out_h = $height-$kernel_h+1;
$out_w = $width-$kernel_w+1;

$host_im_buffer  = new RindowTest\CLBlast\HostBuffer($height*$width*$channels*$batch_count,NDArray::float32);
$host_kernel_buffer = new RindowTest\CLBlast\HostBuffer($kernel_w*$kernel_w*$channels*$num_kernels,NDArray::float32);
$host_result_buffer = new RindowTest\CLBlast\HostBuffer($out_h*$out_w*$num_kernels*$batch_count,NDArray::float32);
for($i=0;$i<count($host_im_buffer);$i++) {
    $host_im_buffer[$i]=$i;
}
for($i=0;$i<count($host_kernel_buffer);$i++) {
    $host_kernel_buffer[$i]=1;
}
$trues = [];
for($i=0;$i<count($host_result_buffer);$i++) {
    $host_result_buffer[$i]=100000;
    $trues[$i]=0;
}
for($b_id=0;$b_id<$batch_count;$b_id++) {
    for($ko_id=0;$ko_id<$num_kernels;$ko_id++) {
        for($ch_id=0;$ch_id<$channels;$ch_id++) {
            for($kh_id=0;$kh_id<$kernel_h;$kh_id++) {
                for($kw_id=0;$kw_id<$kernel_w;$kw_id++) {
                    for($fh_id=0;$fh_id<$out_h;$fh_id++) {
                        for($fw_id=0;$fw_id<$out_w;$fw_id++) {
                            $idx =
                                $b_id*$num_kernels*$out_h*$out_w+
                                $ko_id*$out_h*$out_w+
                                $fh_id*$out_w+
                                $fw_id;
                            $image_index =
                                $b_id*($height*$width*$channels)+
                                $ch_id*($height*$width)+
                                ($kernel_h-$kh_id-1)*($width)+ ###### CAUTION #####
                                ($kernel_w-$kw_id-1)*(1)+      ###### CAUTION #####
                                #$kh_id*($width)+
                                #$kw_id*(1)+
                                $fh_id*($width)+
                                $fw_id*(1);
                            $kernel_index =
                                $ko_id*$channels*$kernel_h*$kernel_w+
                                $ch_id*$kernel_h*$kernel_w+
                                $kh_id*$kernel_w+
                                $kw_id
                                ;
                            $trues[$idx] +=
                                $host_im_buffer[$image_index]*
                                $host_kernel_buffer[$kernel_index];
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
$kernel_buffer = new Rindow\OpenCL\Buffer($context,intval(count($host_kernel_buffer)*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $host_kernel_buffer);
$result_buffer = new Rindow\OpenCL\Buffer($context,intval(count($host_result_buffer)*32/8),
    OpenCL::CL_MEM_WRITE_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $host_result_buffer);

$math = new Rindow\CLBlast\Math();
$events = new Rindow\OpenCL\EventList();
$math->convgemm($kernel_mode,
    $channels,$height,$width,
    $kernel_h,$kernel_w,
    $pad_h,$pad_w,
    $stride_h,$stride_w,
    $dilation_h,$dilation_w,
    $num_kernels,$batch_count,
    $im_buffer,$im_offset,
    $kernel_buffer,$kernel_offset,
    $result_buffer,$result_offset,
    $queue,$events);
$events->wait();
$result_buffer->read($queue,$host_result_buffer);
for($i=0;$i<count($host_result_buffer);$i++) {
    #echo $host_result_buffer[$i].",";
    assert($trues[$i]==$host_result_buffer[$i]);
}
#echo "\n";
#echo "====trues====\n";
#for($i=0;$i<count($host_result_buffer);$i++) {
#    echo $trues[$i].",";
#}
#echo "\n";
echo "SUCCESS convgemm\n";
for($i=0;$i<count($host_result_buffer);$i++) {
    $host_result_buffer[$i]=100000;
}
$result_buffer->write($queue,$host_result_buffer);
$host_col_buffer = new RindowTest\CLBlast\HostBuffer($kernel_h*$kernel_w*$channels*$batch_count*$out_h*$out_w,NDArray::float32);
$col_buffer = new Rindow\OpenCL\Buffer($context,intval(count($host_col_buffer)*32/8),
    OpenCL::CL_MEM_READ_WRITE,null,null,NDArray::float32);
$blas = new Rindow\CLBlast\Blas();
$events = new Rindow\OpenCL\EventList();
for($b=0;$b<$batch_count;$b++) {
    $math->im2col($kernel_mode,
        $channels,$height,$width,
        $kernel_h,$kernel_w,
        $pad_h,$pad_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $im_buffer,$b*$height*$width*$channels,
        $col_buffer,$b*$kernel_h*$kernel_w*4*$channels,
        $queue,$events);
}
$events->wait();
#echo "====== cols =====\n";
#$col_buffer->read($queue,$host_col_buffer);
#for($i=0;$i<count($host_col_buffer);$i++) {
#    echo $host_col_buffer[$i].",";
#    #assert($trues[$i]==$host_result_buffer[$i]);
#}
#echo "\n";
#echo "====== kernel =====\n";
#$kernel_buffer->read($queue,$host_kernel_buffer);
#for($i=0;$i<count($host_kernel_buffer);$i++) {
#    echo $host_kernel_buffer[$i].",";
#    #assert($trues[$i]==$host_result_buffer[$i]);
#}
#echo "\n";
#echo "=================\n";
assert($channels*$kernel_h*$kernel_w*$batch_count*$out_h*$out_w==count($host_col_buffer));
assert($channels*$kernel_h*$kernel_w*$num_kernels==count($host_kernel_buffer));
assert($batch_count*$out_h*$out_w*$num_kernels==count($host_result_buffer));
$m=$num_kernels;
$n=$out_h*$out_w;#*$batch_count;
$k=$channels*$kernel_h*$kernel_w;
#echo "col=(".$m.",".$k."), kernel=(".$k.",".$n."), result=(".$m.",".$n.")\n";
$events = new Rindow\OpenCL\EventList();
for($b=0;$b<$batch_count;$b++) {
    $blas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,
        $m,
        $n,
        $k,
        $alpha=1,
        $kernel_buffer,$kernel_offset,$ldA=$k,
        #$col_buffer,0,$ldB=$n,
        $col_buffer,$b*$n*$k,$ldB=$n,
        $beta=0,
        #$result_buffer,$result_offset,$ldC=$n,
        $result_buffer,$result_offset+$b*$m*$n,$ldC=$n,
        $queue,$events);
}
$events->wait();
$result_buffer->read($queue,$host_result_buffer);
for($i=0;$i<count($host_result_buffer);$i++) {
    #echo $host_result_buffer[$i].",";
    assert($trues[$i]==$host_result_buffer[$i]);
}
#echo "\n";
echo "SUCCESS im2col+gemm\n";
//
// invalid object arguments
//
$events = new Rindow\OpenCL\EventList();
$invalidBuffer = new \stdClass();
try {
    $math->convgemm($kernel_mode,
        $channels,$height,$width,
        $kernel_h,$kernel_w,
        $pad_h,$pad_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $num_kernels,$batch_count,
        $invalidBuffer,$im_offset,
        $kernel_buffer,$kernel_offset,
        $result_buffer,$result_offset,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $math->convgemm($kernel_mode,
        $channels,$height,$width,
        $kernel_h,$kernel_w,
        $pad_h,$pad_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $num_kernels,$batch_count,
        $im_buffer,$im_offset,
        $invalidBuffer,$kernel_offset,
        $result_buffer,$result_offset,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $math->convgemm($kernel_mode,
        $channels,$height,$width,
        $kernel_h,$kernel_w,
        $pad_h,$pad_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $num_kernels,$batch_count,
        $im_buffer,$im_offset,
        $kernel_buffer,$kernel_offset,
        $invalidBuffer,$result_offset,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidQueue = new \stdClass();
try {
    $math->convgemm($kernel_mode,
        $channels,$height,$width,
        $kernel_h,$kernel_w,
        $pad_h,$pad_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $num_kernels,$batch_count,
        $im_buffer,$im_offset,
        $kernel_buffer,$kernel_offset,
        $result_buffer,$result_offset,
        $invalidQueue,$events);
    $blas->axpy(intval(2),$alpha=1.0,
        $bufferX,$offset=0,$inc=1,
        $bufferY,$offsetY=0,$incY=1,
        $invalidQueue,$events);
} catch (\Throwable $e) {
    echo "Invalid Queue catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidEvents = new \stdClass();
try {
    $math->convgemm($kernel_mode,
        $channels,$height,$width,
        $kernel_h,$kernel_w,
        $pad_h,$pad_w,
        $stride_h,$stride_w,
        $dilation_h,$dilation_w,
        $num_kernels,$batch_count,
        $im_buffer,$im_offset,
        $kernel_buffer,$kernel_offset,
        $result_buffer,$result_offset,
        $queue,$invalidEvents);
    $blas->axpy(intval(2),$alpha=1.0,
        $bufferX,$offset=0,$inc=1,
        $bufferY,$offsetY=0,$incY=1,
        $queue,$invalidEvents);
} catch (\Throwable $e) {
    echo "Invalid Event catch: ".get_class($e)."\n";
}
echo "SUCCESS invalid object arguments\n";
?>
--EXPECT--
SUCCESS convgemm
SUCCESS im2col+gemm
Invalid Buffer catch: TypeError
Invalid Buffer catch: TypeError
Invalid Buffer catch: TypeError
Invalid Queue catch: TypeError
Invalid Event catch: TypeError
SUCCESS invalid object arguments
