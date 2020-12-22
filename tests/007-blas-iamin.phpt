--TEST--
Blas::iamin
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
$hostBufferR = new RindowTest\CLBlast\HostBuffer(1,NDArray::int32);
$amin = 100000000;
$iamin = -1;
for($i=0;$i<NMITEM;$i++) {
    $hostBufferX[$i]=$i/NMITEM;
    if(abs($hostBufferX[$i])<$amin) {
        $amin = abs($hostBufferX[$i]);
        $iamin = $i;
    }
}
$bufferX = new Rindow\OpenCL\Buffer($context,intval(NMITEM*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferX);
$bufferR = new Rindow\OpenCL\Buffer($context,intval(64/8),
    OpenCL::CL_MEM_WRITE_ONLY);

$blas = new Rindow\CLBlast\Blas();
$events = new Rindow\OpenCL\EventList();
$blas->iamin(NMITEM,
    $bufferR,$offsetR=0,
    $bufferX,$offsetX=0,$incX=1,
    $queue,$events);
$events->wait();
$bufferR->read($queue,$hostBufferR);
assert($hostBufferR[0]==$iamin);
echo "SUCCESS\n";
?>
--EXPECT--
SUCCESS
