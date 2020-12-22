--TEST--
Blas::gemm
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

$m = 512;
$n = 256;
$k = 256;
$context = new Rindow\OpenCL\Context(OpenCL::CL_DEVICE_TYPE_DEFAULT);
$queue = new Rindow\OpenCL\CommandQueue($context);
$hostBufferA = new RindowTest\CLBlast\HostBuffer($m*$k,NDArray::float32);
$hostBufferB = new RindowTest\CLBlast\HostBuffer($k*$n,NDArray::float32);
$hostBufferC = new RindowTest\CLBlast\HostBuffer($m*$n,NDArray::float32);
$testTruesR  = new RindowTest\CLBlast\HostBuffer($m*$n,NDArray::float32);
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
    $testTruesR[$i]=$hostBufferC[$i];
}
$openblas = new Rindow\OpenBLAS\Blas();
$openblas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
    $alpha,
    $hostBufferA, 0, $k,
    $hostBufferB, 0, $n,
    $beta,
    $testTruesR,  0, $n
);

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
$blas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
    $alpha,
    $bufferA,$offsetA=0,$ldA=$k,
    $bufferB,$offsetB=0,$ldA=$n,
    $beta,
    $bufferC,$offsetC=0,$ldC=$n,
    $queue,$events);
$events->wait();
$bufferC->read($queue,$hostBufferC);
for($i=0;$i<$m*$n;$i++) {
    assert($hostBufferC[$i]==$testTruesR[$i]);
}
echo "SUCCESS\n";
?>
--EXPECT--
SUCCESS
