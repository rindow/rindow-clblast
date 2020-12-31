--TEST--
Blas::gemv
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
$context = new Rindow\OpenCL\Context(OpenCL::CL_DEVICE_TYPE_DEFAULT);
$queue = new Rindow\OpenCL\CommandQueue($context);
$hostBufferA = new RindowTest\CLBlast\HostBuffer($m*$n,NDArray::float32);
$hostBufferX = new RindowTest\CLBlast\HostBuffer($n,NDArray::float32);
$hostBufferY = new RindowTest\CLBlast\HostBuffer($m,NDArray::float32);
$testTruesR = new RindowTest\CLBlast\HostBuffer($m,NDArray::float32);
$alpha=2.0;
$beta=0.5;
for($i=0;$i<$m*$n;$i++) {
    $hostBufferA[$i]=random_int(0, 255)/256;
}
for($i=0;$i<$n;$i++) {
    $hostBufferX[$i]=random_int(0, 255)/256;
}
for($i=0;$i<$m;$i++) {
    $hostBufferY[$i]=random_int(0, 255)/256;
    $testTruesR[$i]=$hostBufferY[$i];
}
$openblas = new Rindow\OpenBLAS\Blas();
$openblas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
    $alpha,
    $hostBufferA, 0, $n,
    $hostBufferX, 0, 1,
    $beta,
    $testTruesR,  0, 1
);

$bufferA = new Rindow\OpenCL\Buffer($context,intval($m*$n*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferA);
$bufferX = new Rindow\OpenCL\Buffer($context,intval($n*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferX);
$bufferY = new Rindow\OpenCL\Buffer($context,intval($m*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferY);

$blas = new Rindow\CLBlast\Blas();
$events = new Rindow\OpenCL\EventList();
$blas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
    $alpha,
    $bufferA,$offsetA=0,$ldA=$n,
    $bufferX,$offsetX=0,$incX=1,
    $beta,
    $bufferY,$offsetY=0,$incY=1,
    $queue,$events);
$events->wait();
$bufferY->read($queue,$hostBufferY);
for($i=0;$i<$m;$i++) {
    assert($hostBufferY[$i]==$testTruesR[$i]);
}
echo "SUCCESS\n";
//
// invalid object arguments
//
$events = new Rindow\OpenCL\EventList();
$invalidBuffer = new \stdClass();
try {
    $blas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
        $alpha,
        $invalidBuffer,$offsetA=0,$ldA=$n,
        $bufferX,$offsetX=0,$incX=1,
        $beta,
        $bufferY,$offsetY=0,$incY=1,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $blas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
        $alpha,
        $bufferA,$offsetA=0,$ldA=$n,
        $invalidBuffer,$offsetX=0,$incX=1,
        $beta,
        $bufferY,$offsetY=0,$incY=1,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $blas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
        $alpha,
        $bufferA,$offsetA=0,$ldA=$n,
        $bufferX,$offsetX=0,$incX=1,
        $beta,
        $invalidBuffer,$offsetY=0,$incY=1,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidQueue = new \stdClass();
try {
    $blas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
        $alpha,
        $bufferA,$offsetA=0,$ldA=$n,
        $bufferX,$offsetX=0,$incX=1,
        $beta,
        $bufferY,$offsetY=0,$incY=1,
        $invalidQueue,$events);
} catch (\Throwable $e) {
    echo "Invalid Queue catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidEvents = new \stdClass();
try {
    $blas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
        $alpha,
        $bufferA,$offsetA=0,$ldA=$n,
        $bufferX,$offsetX=0,$incX=1,
        $beta,
        $bufferY,$offsetY=0,$incY=1,
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
Invalid Buffer catch: TypeError
Invalid Queue catch: TypeError
Invalid Event catch: TypeError
SUCCESS invalid object arguments
