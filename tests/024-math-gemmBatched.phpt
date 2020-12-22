--TEST--
Blas::gemmBatched
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
use Interop\Polite\Math\Matrix\BLAS;
$batch_count = 4;
$m = 8;
$n = 6;
$k = 4;
$context = new Rindow\OpenCL\Context(OpenCL::CL_DEVICE_TYPE_DEFAULT);
$queue = new Rindow\OpenCL\CommandQueue($context);
$hostAlpha   = new RindowTest\CLBlast\HostBuffer($batch_count,NDArray::float32);
$hostBeta    = new RindowTest\CLBlast\HostBuffer($batch_count,NDArray::float32);
$hostBufferA = new RindowTest\CLBlast\HostBuffer($batch_count*$m*$k,NDArray::float32);
$hostBufferB = new RindowTest\CLBlast\HostBuffer($batch_count*$k*$n,NDArray::float32);
$hostBufferC = new RindowTest\CLBlast\HostBuffer($batch_count*$m*$n,NDArray::float32);
$testTruesR  = new RindowTest\CLBlast\HostBuffer($batch_count*$m*$n,NDArray::float32);
$offsetsA = new RindowTest\CLBlast\HostBuffer($batch_count,NDArray::int64);
$offsetsB = new RindowTest\CLBlast\HostBuffer($batch_count,NDArray::int64);
$offsetsC = new RindowTest\CLBlast\HostBuffer($batch_count,NDArray::int64);

for($ii=0;$ii<$batch_count;$ii++) {
    $hostAlpha[$ii] = random_int(0, 255)/256;
    for($i=0;$i<$m*$k;$i++) {
        $hostBufferA[$ii*$m*$k+$i]=random_int(0, 255)/256;
    }
    for($i=0;$i<$k*$n;$i++) {
        $hostBufferB[$ii*$k*$n+$i]=random_int(0, 255)/256;
    }
    $hostBeta[$ii] = random_int(0, 255)/256;
    for($i=0;$i<$m*$n;$i++) {
        $hostBufferC[$ii*$m*$n+$i]=random_int(0, 255)/256;
        $testTruesR[$ii*$m*$n+$i]=$hostBufferC[$ii*$m*$n+$i];
    }
}

$openmath = new Rindow\OpenBLAS\Math();
$openmath->randomSequence($batch_count,$batch_count,$offsetsA, 0, 1,random_int(~PHP_INT_MAX,PHP_INT_MAX));
$openmath->randomSequence($batch_count,$batch_count,$offsetsB, 0, 1,random_int(~PHP_INT_MAX,PHP_INT_MAX));
$openmath->randomSequence($batch_count,$batch_count,$offsetsC, 0, 1,random_int(~PHP_INT_MAX,PHP_INT_MAX));
#echo "offsetsA=";
for($ii=0;$ii<$batch_count;$ii++) {
    #echo $offsetsA[$ii] .",";
    $offsetsA[$ii] = $offsetsA[$ii]*$m*$k;
}
#echo "\n";
#echo "offsetsB=";
for($ii=0;$ii<$batch_count;$ii++) {
    #echo $offsetsB[$ii].",";
    $offsetsB[$ii] = $offsetsB[$ii]*$k*$n;
}
#echo "\n";
#echo "offsetsC=";
for($ii=0;$ii<$batch_count;$ii++) {
    #echo $offsetsC[$ii].",";
    $offsetsC[$ii] = $offsetsC[$ii]*$m*$n;
}
#echo "\n";
$openblas = new Rindow\OpenBLAS\Blas();
for($ii=0;$ii<$batch_count;$ii++) {
    $openblas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
        $hostAlpha[$ii],
        $hostBufferA, $offsetsA[$ii], $k,
        $hostBufferB, $offsetsB[$ii], $n,
        $hostBeta[$ii],
        $testTruesR,  $offsetsC[$ii], $n
    );
}

$bufferA = new Rindow\OpenCL\Buffer($context,intval(count($hostBufferA)*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferA);
$bufferB = new Rindow\OpenCL\Buffer($context,intval(count($hostBufferB)*32/8),
    OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferB);
$bufferC = new Rindow\OpenCL\Buffer($context,intval(count($hostBufferC)*32/8),
    OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
    $hostBufferC);

$math = new Rindow\CLBlast\Math();
$events = new Rindow\OpenCL\EventList();
$math->gemmBatched(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
    $hostAlpha,0,
    $bufferA,$offsetsA,$offsetA=0,$ldA=$k,
    $bufferB,$offsetsB,$offsetB=0,$ldA=$n,
    $hostBeta,0,
    $bufferC,$offsetsC,$offsetC=0,$ldC=$n,
    $batch_count,
    $queue,$events);
$events->wait();
$bufferC->read($queue,$hostBufferC);
for($ii=0;$ii<$batch_count;$ii++) {
    for($i=0;$i<$m*$n;$i++) {
        #echo $hostBufferC[$ii*$m*$n+$i].",";
        assert($hostBufferC[$ii*$m*$n+$i]==$testTruesR[$ii*$m*$n+$i]);
    }
    #echo "\n";
}
echo "SUCCESS\n";
?>
--EXPECT--
SUCCESS
