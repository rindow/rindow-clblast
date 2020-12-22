--TEST--
Math::imax
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
$hostBuffer = new RindowTest\CLBlast\HostBuffer(NMITEM,NDArray::float32);
$imax = 0.0;
for($i=0;$i<NMITEM;$i++) {
    $hostBuffer[$i]=$i-NMITEM/2;
}
$buffer = new Rindow\OpenCL\Buffer($context,intval(NMITEM*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBuffer);
$bufferR = new Rindow\OpenCL\Buffer($context,intval(32/8),
    OpenCL::CL_MEM_READ_WRITE);

$math = new Rindow\CLBlast\Math();
$events = new Rindow\OpenCL\EventList();
$math->imax(NMITEM,$bufferR,$offsetR=0,$buffer,$offset=0,$inc=1,$queue,$events);
$events->wait();
$hostBufferR = new RindowTest\CLBlast\HostBuffer(1,NDArray::int32);
$bufferR->read($queue,$hostBufferR,intval(32/8));
assert($hostBufferR[0]==NMITEM-1);
echo "SUCCESS\n";
?>
--EXPECT--
SUCCESS
