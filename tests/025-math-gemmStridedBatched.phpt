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
$strideA = $m*$k;
$strideB = $k*$n;
$strideC = $m*$n;
$context = new Rindow\OpenCL\Context(OpenCL::CL_DEVICE_TYPE_DEFAULT);
$queue = new Rindow\OpenCL\CommandQueue($context);
$hostBufferA = new RindowTest\CLBlast\HostBuffer($batch_count*$m*$k,NDArray::float32);
$hostBufferB = new RindowTest\CLBlast\HostBuffer($batch_count*$k*$n,NDArray::float32);
$hostBufferC = new RindowTest\CLBlast\HostBuffer($batch_count*$m*$n,NDArray::float32);
$testTruesR  = new RindowTest\CLBlast\HostBuffer($batch_count*$m*$n,NDArray::float32);
$alpha=2.0;
$beta=0.5;

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

$openblas = new Rindow\OpenBLAS\Blas();
for($ii=0;$ii<$batch_count;$ii++) {
    $openblas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
        $alpha,
        $hostBufferA, $strideA*$ii, $k,
        $hostBufferB, $strideB*$ii, $n,
        $beta,
        $testTruesR,  $strideC*$ii, $n
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
$math->gemmStridedBatched(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
    $alpha,
    $bufferA,$offsetA=0,$ldA=$k,$strideA,
    $bufferB,$offsetB=0,$ldA=$n,$strideB,
    $beta,
    $bufferC,$offsetC=0,$ldC=$n,$strideC,
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
//
// invalid object arguments
//
$events = new Rindow\OpenCL\EventList();
$invalidBuffer = new \stdClass();
try {
    $math->gemmStridedBatched(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
        $alpha,
        $invalidBuffer,$offsetA=0,$ldA=$k,$strideA,
        $bufferB,$offsetB=0,$ldA=$n,$strideB,
        $beta,
        $bufferC,$offsetC=0,$ldC=$n,$strideC,
        $batch_count,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $math->gemmStridedBatched(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
        $alpha,
        $bufferA,$offsetA=0,$ldA=$k,$strideA,
        $invalidBuffer,$offsetB=0,$ldA=$n,$strideB,
        $beta,
        $bufferC,$offsetC=0,$ldC=$n,$strideC,
        $batch_count,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
try {
    $math->gemmStridedBatched(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
        $alpha,
        $bufferA,$offsetA=0,$ldA=$k,$strideA,
        $bufferB,$offsetB=0,$ldA=$n,$strideB,
        $beta,
        $invalidBuffer,$offsetC=0,$ldC=$n,$strideC,
        $batch_count,
        $queue,$events);
} catch (\Throwable $e) {
    echo "Invalid Buffer catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidQueue = new \stdClass();
try {
    $math->gemmStridedBatched(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
        $alpha,
        $bufferA,$offsetA=0,$ldA=$k,$strideA,
        $bufferB,$offsetB=0,$ldA=$n,$strideB,
        $beta,
        $bufferC,$offsetC=0,$ldC=$n,$strideC,
        $batch_count,
        $invalidQueue,$events);
} catch (\Throwable $e) {
    echo "Invalid Queue catch: ".get_class($e)."\n";
}
$events = new Rindow\OpenCL\EventList();
$invalidEvents = new \stdClass();
try {
    $math->gemmStridedBatched(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
        $alpha,
        $bufferA,$offsetA=0,$ldA=$k,$strideA,
        $bufferB,$offsetB=0,$ldA=$n,$strideB,
        $beta,
        $bufferC,$offsetC=0,$ldC=$n,$strideC,
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
Invalid Queue catch: TypeError
Invalid Event catch: TypeError
SUCCESS invalid object arguments
