--TEST--
Blas::scal
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
for($i=0;$i<NMITEM;$i++) {
    $hostBuffer[$i]=$i;
}
$buffer = new Rindow\OpenCL\Buffer($context,intval(NMITEM*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBuffer);

$blas = new Rindow\CLBlast\Blas();
$events = new Rindow\OpenCL\EventList();
$blas->scal(NMITEM,$alpha=2.0,$buffer,$offset=0,$inc=1,$queue,$events);
$events->wait();
$buffer->read($queue,$hostBuffer);
for($i=0;$i<NMITEM;$i++) {
    assert($hostBuffer[$i]==$i*2);
}
echo "SUCCESS Full-range\n";
$blas->scal(intval(NMITEM/2),$alpha=0.5,$buffer,
            $offset=intval(NMITEM/2),$inc=1,$queue,$events);
$events->wait();
$buffer->read($queue,$hostBuffer);
for($i=0;$i<NMITEM;$i++) {
    if($i<intval(NMITEM/2))
        assert($hostBuffer[$i]==$i*2);
    else
        assert($hostBuffer[$i]==$i);
}
echo "SUCCESS Offset-range\n";
$blas->scal(intval(NMITEM/2),$alpha=0.5,$buffer,
            $offset=0,$inc=1,$queue,$events);
$events->wait();
$buffer->read($queue,$hostBuffer);
for($i=0;$i<NMITEM;$i++) {
    assert($hostBuffer[$i]==$i);
}
echo "SUCCESS Limit-range\n";
//
// invalid object arguments
//
$events = new Rindow\OpenCL\EventList();
$invalidBuffer = new \stdClass();
try {
    $blas->scal(intval(2),$alpha=1.0,
        $invalidBuffer,$offset=0,$inc=1,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidQueue = new \stdClass();
try {
    $blas->scal(intval(2),$alpha=1.0,
        $buffer,$offset=0,$inc=1,
        $invalidQueue,$events);
} catch (\Throwable $e) {
    echo "Invalid Queue catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidEvents = new \stdClass();
try {
    $blas->scal(intval(2),$alpha=1.0,
        $buffer,$offset=0,$inc=1,
        $queue,$invalidEvents);
} catch (\Throwable $e) {
    echo "Invalid Event catch: ".get_class($e)."\n";
}
echo "SUCCESS invalid object arguments\n";
?>
--EXPECT--
SUCCESS Full-range
SUCCESS Offset-range
SUCCESS Limit-range
Invalid Buffer catch: TypeError
Invalid Queue catch: TypeError
Invalid Event catch: TypeError
SUCCESS invalid object arguments
