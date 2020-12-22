--TEST--
Blas::swap
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
define('NMITEMS',1024);

$context = new Rindow\OpenCL\Context(OpenCL::CL_DEVICE_TYPE_DEFAULT);
$queue = new Rindow\OpenCL\CommandQueue($context);
$hostBufferX = new RindowTest\CLBlast\HostBuffer(NMITEMS,NDArray::float32);
$hostBufferY = new RindowTest\CLBlast\HostBuffer(NMITEMS,NDArray::float32);

for($i=0;$i<NMITEMS;$i++) {
    $hostBufferX[$i] = $i;
    $hostBufferY[$i] = NMITEMS-$i;
}

$bufferX = new Rindow\OpenCL\Buffer($context,intval(NMITEMS*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferX);
$bufferY = new Rindow\OpenCL\Buffer($context,intval(NMITEMS*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferY);

$blas = new Rindow\CLBlast\Blas();
$events = new Rindow\OpenCL\EventList();
$blas->swap(
    NMITEMS,
    $bufferX,$offsetA=0,$incX=1,
    $bufferY,$offsetY=0,$incY=1,
    $queue,$events);
$events->wait();
$bufferX->read($queue,$hostBufferX);
$bufferY->read($queue,$hostBufferY);
for($i=0;$i<NMITEMS;$i++) {
    assert($hostBufferX[$i] == NMITEMS-$i);
    assert($hostBufferY[$i] == $i);
}

echo "SUCCESS\n";
?>
--EXPECT--
SUCCESS
