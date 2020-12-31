--TEST--
Blas::axpyBatched
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
define('NMITEM',3);
$batch_count = 4;
$context = new Rindow\OpenCL\Context(OpenCL::CL_DEVICE_TYPE_DEFAULT);
$queue = new Rindow\OpenCL\CommandQueue($context);
$hostBufferX = new RindowTest\CLBlast\HostBuffer(NMITEM*$batch_count,NDArray::float32);
$hostBufferY = new RindowTest\CLBlast\HostBuffer(NMITEM*$batch_count,NDArray::float32);
$hostAlphas  = new RindowTest\CLBlast\HostBuffer($batch_count,NDArray::float32);
$hostOffsetsX = new RindowTest\CLBlast\HostBuffer($batch_count,NDArray::int64);
$hostOffsetsY = new RindowTest\CLBlast\HostBuffer($batch_count,NDArray::int64);
for($i=0;$i<$batch_count;$i++) {
    $hostAlphas[$i]=$batch_count-$i;
    $hostOffsetsX[$i]=($batch_count-$i-1)*NMITEM;
    $hostOffsetsY[$i]=$i*NMITEM;
}
for($i=0;$i<$batch_count;$i++) {
    for($j=0;$j<NMITEM;$j++) {
        $hostBufferX[$i*NMITEM+$j]=($i+1)*1000;#$j+$i*1000;
        $hostBufferY[$i*NMITEM+$j]=($i+1)*100000;#$j+$i*100000;
    }
}
$bufferX = new Rindow\OpenCL\Buffer($context,intval(count($hostBufferX)*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferX);
$bufferY = new Rindow\OpenCL\Buffer($context,intval(count($hostBufferY)*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferY);

#echo "X:\n";
#for($i=0;$i<$batch_count;$i++) {
#    for($j=0;$j<NMITEM;$j++) {
#        echo $hostBufferX[$i*NMITEM+$j],",";
#    }
#    echo "\n";
#}
#echo "Y:\n";
#for($i=0;$i<$batch_count;$i++) {
#    for($j=0;$j<NMITEM;$j++) {
#        echo $hostBufferY[$i*NMITEM+$j],",";
#    }
#    echo "\n";
#}

$math = new Rindow\CLBlast\Math();
$events = new Rindow\OpenCL\EventList();
$math->axpyBatched(NMITEM,
    $hostAlphas,$offsetA=0,
    $bufferX,$hostOffsetsX,$offsetX=0,$incX=1,
    $bufferY,$hostOffsetsY,$offsetY=0,$incY=1,
    $batch_count,
    $queue,$events);
$events->wait();
$bufferY->read($queue,$hostBufferY);
#echo "R:\n";
for($i=0;$i<$batch_count;$i++) {
    for($j=0;$j<NMITEM;$j++) {
        #echo $hostBufferY[$i*NMITEM+$j],",";
        assert($hostBufferY[$i*NMITEM+$j]==(pow($batch_count-$i,2)*1000)+($i+1)*100000);
    }
    #echo "\n";
}
echo "SUCCESS\n";
//
// invalid object arguments
//
$events = new Rindow\OpenCL\EventList();
$invalidBuffer = new \stdClass();
try {
    $math->axpyBatched(NMITEM,
        $invalidBuffer,$offsetA=0,
        $bufferX,$hostOffsetsX,$offsetX=0,$incX=1,
        $bufferY,$hostOffsetsY,$offsetY=0,$incY=1,
        $batch_count,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $math->axpyBatched(NMITEM,
        $hostAlphas,$offsetA=0,
        $invalidBuffer,$hostOffsetsX,$offsetX=0,$incX=1,
        $bufferY,$hostOffsetsY,$offsetY=0,$incY=1,
        $batch_count,
        $queue,$events);
    $blas->axpy(intval(2),$alpha=1.0,
        $bufferX,$offset=0,$inc=1,
        $invalidBuffer,$offsetY=0,$incY=1,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $math->axpyBatched(NMITEM,
        $hostAlphas,$offsetA=0,
        $bufferX,$invalidBuffer,$offsetX=0,$incX=1,
        $bufferY,$hostOffsetsY,$offsetY=0,$incY=1,
        $batch_count,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $math->axpyBatched(NMITEM,
        $hostAlphas,$offsetA=0,
        $bufferX,$hostOffsetsX,$offsetX=0,$incX=1,
        $invalidBuffer,$hostOffsetsY,$offsetY=0,$incY=1,
        $batch_count,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $math->axpyBatched(NMITEM,
        $hostAlphas,$offsetA=0,
        $bufferX,$hostOffsetsX,$offsetX=0,$incX=1,
        $bufferY,$invalidBuffer,$offsetY=0,$incY=1,
        $batch_count,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidQueue = new \stdClass();
try {
    $math->axpyBatched(NMITEM,
        $hostAlphas,$offsetA=0,
        $bufferX,$hostOffsetsX,$offsetX=0,$incX=1,
        $bufferY,$hostOffsetsY,$offsetY=0,$incY=1,
        $batch_count,
        $invalidQueue,$events);
} catch (\Throwable $e) {
    echo "Invalid Queue catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidEvents = new \stdClass();
try {
    $math->axpyBatched(NMITEM,
        $hostAlphas,$offsetA=0,
        $bufferX,$hostOffsetsX,$offsetX=0,$incX=1,
        $bufferY,$hostOffsetsY,$offsetY=0,$incY=1,
        $batch_count,
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
Invalid Buffer catch: TypeError
Invalid Buffer catch: TypeError
Invalid Queue catch: TypeError
Invalid Event catch: TypeError
SUCCESS invalid object arguments
