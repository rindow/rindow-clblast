--TEST--
Math::hadamard
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
define('NMITEM',2048);

$context = new Rindow\OpenCL\Context(OpenCL::CL_DEVICE_TYPE_DEFAULT);
$queue = new Rindow\OpenCL\CommandQueue($context);
$hostBufferX = new RindowTest\CLBlast\HostBuffer(NMITEM,NDArray::float32);
$hostBufferY = new RindowTest\CLBlast\HostBuffer(NMITEM,NDArray::float32);
$hostBufferZ = new RindowTest\CLBlast\HostBuffer(NMITEM,NDArray::float32);
$hostBufferR = new RindowTest\CLBlast\HostBuffer(NMITEM,NDArray::float32);
$alpha = 2.0;
$beta = 3.0;
for($i=0;$i<NMITEM;$i++) {
    $hostBufferX[$i]=$i-NMITEM/2;
    $hostBufferY[$i]=NMITEM/2-$i;
    $hostBufferZ[$i]=$i;
    $hostBufferR[$i]=$alpha*($i-NMITEM/2)*(NMITEM/2-$i)+$beta*$i;
}
$bufferX = new Rindow\OpenCL\Buffer($context,intval(NMITEM*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferX);
$bufferY = new Rindow\OpenCL\Buffer($context,intval(NMITEM*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferY);
$bufferZ = new Rindow\OpenCL\Buffer($context,intval(NMITEM*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferZ);

$math = new Rindow\CLBlast\Math();
$events = new Rindow\OpenCL\EventList();
$math->hadamard(NMITEM,$alpha,$bufferX,$offsetX=0,$incX=1,
    $bufferY,$offsetY=0,$incY=1,$beta,$bufferZ,$offsetZ=0,$incZ=1,$queue,$events);
$events->wait();
$bufferZ->read($queue,$hostBufferZ,intval(NMITEM*32/8));
for($i=0;$i<NMITEM;$i++) {
    assert($hostBufferR[$i]==$hostBufferZ[$i]);
}
echo "SUCCESS\n";
?>
--EXPECT--
SUCCESS
