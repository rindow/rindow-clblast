--TEST--
Blas::copy
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
$blas->copy(NMITEM,
    $bufferX,$offsetX=0,$incX=1,
    $bufferY,$offsetY=0,$incY=1,
    $queue,$events);
$events->wait();
$bufferY->read($queue,$hostBufferY);
for($i=0;$i<NMITEM;$i++) {
    assert($hostBufferY[$i]==$i);
}
echo "SUCCESS float\n";
$hostBufferX = new RindowTest\CLBlast\HostBuffer(NMITEM,NDArray::int8);
$hostBufferY = new RindowTest\CLBlast\HostBuffer(NMITEM,NDArray::int8);
for($i=0;$i<NMITEM;$i++) {
    $hostBufferX[$i]=$i;
    $hostBufferY[$i]=NMITEM-1-$i;
}
$bufferX = new Rindow\OpenCL\Buffer($context,intval(NMITEM*8/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferX);
$bufferY = new Rindow\OpenCL\Buffer($context,intval(NMITEM*8/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferY);
$events = new Rindow\OpenCL\EventList();
$blas->copy(NMITEM,
    $bufferX,$offsetX=0,$incX=1,
    $bufferY,$offsetY=0,$incY=1,
    $queue,$events);
$events->wait();
$bufferY->read($queue,$hostBufferY);
for($i=0;$i<NMITEM;$i++) {
    assert( $hostBufferY[$i]==(($i+128)%256)-128 );
}
echo "SUCCESS int8\n";
$hostBufferX = new RindowTest\CLBlast\HostBuffer(NMITEM,NDArray::int64);
$hostBufferY = new RindowTest\CLBlast\HostBuffer(NMITEM,NDArray::int64);
for($i=0;$i<NMITEM;$i++) {
    $hostBufferX[$i]=$i;
    $hostBufferY[$i]=NMITEM-1-$i;
}
$bufferX = new Rindow\OpenCL\Buffer($context,intval(NMITEM*64/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferX);
$bufferY = new Rindow\OpenCL\Buffer($context,intval(NMITEM*64/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferY);
$events = new Rindow\OpenCL\EventList();
$blas->copy(NMITEM,
    $bufferX,$offsetX=0,$incX=1,
    $bufferY,$offsetY=0,$incY=1,
    $queue,$events);
$events->wait();
$bufferY->read($queue,$hostBufferY);
for($i=0;$i<NMITEM;$i++) {
    assert($hostBufferY[$i]==$i);
}
echo "SUCCESS int64\n";
//
// invalid object arguments
//
$events = new Rindow\OpenCL\EventList();
$invalidBuffer = new \stdClass();
try {
    $blas->copy(NMITEM,
        $invalidBuffer,$offsetX=0,$incX=1,
        $bufferY,$offsetY=0,$incY=1,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $blas->copy(NMITEM,
        $bufferX,$offsetX=0,$incX=1,
        $invalidBuffer,$offsetY=0,$incY=1,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidQueue = new \stdClass();
try {
    $blas->copy(NMITEM,
        $bufferX,$offsetX=0,$incX=1,
        $bufferY,$offsetY=0,$incY=1,
        $invalidQueue,$events);
} catch (\Throwable $e) {
    echo "Invalid Queue catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidEvents = new \stdClass();
try {
    $blas->copy(NMITEM,
        $bufferX,$offsetX=0,$incX=1,
        $bufferY,$offsetY=0,$incY=1,
        $queue,$invalidEvents);
} catch (\Throwable $e) {
    echo "Invalid Event catch: ".get_class($e)."\n";
}
echo "SUCCESS invalid object arguments\n";
?>
--EXPECT--
SUCCESS float
SUCCESS int8
SUCCESS int64
Invalid Buffer catch: TypeError
Invalid Buffer catch: TypeError
Invalid Queue catch: TypeError
Invalid Event catch: TypeError
SUCCESS invalid object arguments
