--TEST--
Blas::nrm2
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
define('NMITEM',1024);

$context = new Rindow\OpenCL\Context(OpenCL::CL_DEVICE_TYPE_DEFAULT);
$queue = new Rindow\OpenCL\CommandQueue($context);
$hostBufferX = new RindowTest\CLBlast\HostBuffer(NMITEM,NDArray::float32);
$hostBufferR = new RindowTest\CLBlast\HostBuffer(1,NDArray::float32);
$nrm2 = 0.0;
for($i=0;$i<NMITEM;$i++) {
    $hostBufferX[$i]=$i/NMITEM;
}
$openblas = new Rindow\OpenBLAS\Blas();
$nrm2 = $openblas->nrm2(NMITEM,$hostBufferX, 0, 1);

$bufferX = new Rindow\OpenCL\Buffer($context,intval(NMITEM*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferX);
$bufferR = new Rindow\OpenCL\Buffer($context,intval(32/8),
    OpenCL::CL_MEM_WRITE_ONLY);

$blas = new Rindow\CLBlast\Blas();
$events = new Rindow\OpenCL\EventList();
$blas->nrm2(NMITEM,
    $bufferR,$offsetR=0,
    $bufferX,$offsetX=0,$incX=1,
    $queue,$events);
$events->wait();
$bufferR->read($queue,$hostBufferR);
assert(abs($hostBufferR[0]-$nrm2)<1e-7);
echo "SUCCESS\n";
//
// invalid object arguments
//
$events = new Rindow\OpenCL\EventList();
$invalidBuffer = new \stdClass();
try {
    $blas->nrm2(NMITEM,
        $invalidBuffer,$offsetR=0,
        $bufferX,$offsetX=0,$incX=1,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $blas->nrm2(NMITEM,
        $bufferR,$offsetR=0,
        $invalidBuffer,$offsetX=0,$incX=1,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidQueue = new \stdClass();
try {
    $blas->nrm2(NMITEM,
        $bufferR,$offsetR=0,
        $bufferX,$offsetX=0,$incX=1,
        $invalidQueue,$events);
} catch (\Throwable $e) {
    echo "Invalid Queue catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidEvents = new \stdClass();
try {
    $blas->nrm2(NMITEM,
        $bufferR,$offsetR=0,
        $bufferX,$offsetX=0,$incX=1,
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
