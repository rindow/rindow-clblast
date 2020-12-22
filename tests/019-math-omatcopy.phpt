--TEST--
Math::omatcopy
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
use Interop\Polite\Math\Matrix\BLAS;
use Interop\Polite\Math\Matrix\NDArray;
$m=256;
$n=128;
$context = new Rindow\OpenCL\Context(OpenCL::CL_DEVICE_TYPE_DEFAULT);
$queue = new Rindow\OpenCL\CommandQueue($context);
$hostBufferA = new RindowTest\CLBlast\HostBuffer($n*$m,NDArray::float32);
$hostBufferB = new RindowTest\CLBlast\HostBuffer($m*$n,NDArray::float32);
$hostBufferR = new RindowTest\CLBlast\HostBuffer($m*$n,NDArray::float32);
$alpha = 2.0;
for($i=0;$i<$m*$n;$i++) {
    $hostBufferA[$i]=$i;
}
for($i=0;$i<$n;$i++) {
    for($j=0;$j<$m;$j++) {
        $hostBufferB[$i*$m+$j]=10000;
        $hostBufferR[$i*$m+$j]=$alpha*($j*$n+$i);
    }
}
$bufferA = new Rindow\OpenCL\Buffer($context,intval($m*$n*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferA);
$bufferB = new Rindow\OpenCL\Buffer($context,intval($m*$n*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferB);

$math = new Rindow\CLBlast\Math();
$events = new Rindow\OpenCL\EventList();
$math->omatcopy(BLAS::RowMajor,BLAS::Trans,$m,$n,$alpha,
    $bufferA,$offsetA=0,$ldA=$n,
    $bufferB,$offsetB=0,$ldB=$m,
    $queue,$events);
$events->wait();
$bufferB->read($queue,$hostBufferB,intval($m*$n*32/8));
for($i=0;$i<$n;$i++) {
    for($j=0;$j<$m;$j++) {
        assert($hostBufferB[$i*$m+$j]==$hostBufferR[$i*$m+$j]);
    }
    echo "\n";
}
echo "SUCCESS\n";
?>
--EXPECT--
SUCCESS
