--TEST--
Blas::gemm no wait
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
use Interop\Polite\Math\Matrix\NDArray;
use Interop\Polite\Math\Matrix\BLAS;

//$m = 2048;
//$n = 2048;
//$k = 2048;
$m = 512;
$n = 512;
$k = 512;
$context = new Rindow\OpenCL\Context(OpenCL::CL_DEVICE_TYPE_DEFAULT);
$queue = new Rindow\OpenCL\CommandQueue($context);
$hostBufferA = new RindowTest\CLBlast\HostBuffer($m*$k,NDArray::float32);
$hostBufferB = new RindowTest\CLBlast\HostBuffer($k*$n,NDArray::float32);
$hostBufferC = new RindowTest\CLBlast\HostBuffer($m*$n,NDArray::float32);
$alpha=2.0;
$beta=0.5;
for($i=0;$i<$m*$k;$i++) {
    $hostBufferA[$i]=random_int(0, 255)/256;
}
for($i=0;$i<$k*$n;$i++) {
    $hostBufferB[$i]=random_int(0, 255)/256;
}
for($i=0;$i<$m*$n;$i++) {
    $hostBufferC[$i]=random_int(0, 255)/256;
}
$bufferA = new Rindow\OpenCL\Buffer($context,intval($m*$k*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferA);
$bufferB = new Rindow\OpenCL\Buffer($context,intval($k*$n*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferB);
$bufferC = new Rindow\OpenCL\Buffer($context,intval($m*$n*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferC);

$blas = new Rindow\CLBlast\Blas();
$events = new Rindow\OpenCL\EventList();
#$start = hrtime(true);
$start = microtime(true);
for($i=0;$i<5;$i++) {
    $blas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
        $alpha,
        $bufferA,$offsetA=0,$ldA=$k,
        $bufferB,$offsetB=0,$ldA=$n,
        $beta,
        $bufferC,$offsetC=0,$ldC=$n,
        $queue);#,$events);
    #$events->wait();
}
$enq = microtime(true);
$queue->finish();
$end = microtime(true);
#echo "done\n";
#echo "total time=".($end-$start)."\n";
#echo "enqueue time=".($enq-$start)."\n";
#echo "wait time=".($end-$enq)."\n";
#$end = hrtime(true);
#$bufferC->read($queue,$hostBufferC);
echo "SUCCESS\n";
?>
--EXPECT--
SUCCESS
