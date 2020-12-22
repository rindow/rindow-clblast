--TEST--
Blas::axpy
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
$hostBufferY = new RindowTest\CLBlast\HostBuffer(NMITEM,NDArray::float32);
$alpha=2.0;
for($i=0;$i<NMITEM;$i++) {
    $hostBufferX[$i]=$i;
    $hostBufferY[$i]=NMITEM-1-$i;
}
$bufferX = new Rindow\OpenCL\Buffer($context,intval(NMITEM*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferX);
$bufferY = new Rindow\OpenCL\Buffer($context,intval(NMITEM*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferY);

$blas = new Rindow\CLBlast\Blas();
$events = new Rindow\OpenCL\EventList();
$blas->axpy(NMITEM,$alpha,
    $bufferX,$offsetX=0,$incX=1,
    $bufferY,$offsetY=0,$incY=1,
    $queue,$events);
$events->wait();
$bufferY->read($queue,$hostBufferY);
for($i=0;$i<NMITEM;$i++) {
    assert($hostBufferY[$i]==($i*2)+(NMITEM-1-$i));
}
echo "SUCCESS\n";
?>
--EXPECT--
SUCCESS
