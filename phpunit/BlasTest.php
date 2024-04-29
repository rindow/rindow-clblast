<?php
namespace RindowTest\CLBlast\BlasTest;

use PHPUnit\Framework\TestCase;
use Interop\Polite\Math\Matrix\NDArray;
use Interop\Polite\Math\Matrix\BLAS;
use Interop\Polite\Math\Matrix\OpenCL;

use RindowTest\CLBlast\HostBuffer;
use Rindow\CLBlast\Blas as CLBlastBLAS;
use Rindow\OpenBLAS\Blas as OpenBLAS;
use InvalidArgumentException;
use RuntimeException;
use TypeError;
use ArrayObject;
use ArrayAccess;

require_once __DIR__.'/../testPHP/HostBuffer.php';
require_once __DIR__.'/OpenCLFactory.php';
require_once __DIR__.'/Utils.php';
use RindowTest\CLBlast\Utils;
use function RindowTest\CLBlast\FFI\C;

class BlasTest extends TestCase
{
    use Utils;

    protected $skipDisplayInfo = true;

    //protected int $defaultDeviceType = OpenCL::CL_DEVICE_TYPE_DEFAULT;
    //protected int $defaultDeviceType = OpenCL::CL_DEVICE_TYPE_GPU;
    static protected $default_device_type = OpenCL::CL_DEVICE_TYPE_GPU;

    protected $opencl=null;
    protected $clblast=null;
    protected $openblas=null;

    public function setUp() : void
    {
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $this->setOpenCLQueue($queue);
    }

    public function getBlas()
    {
        return new CLBlastBlas();
    }

    public function getOpenBLAS()
    {
        return new OpenBLAS();
    }

    public function newContextFromType($ocl)
    {
        try {
            $context = $ocl->Context(self::$default_device_type);
        } catch(RuntimeException $e) {
            if(strpos('clCreateContextFromType',$e->getMessage())===null) {
                throw $e;
            }
            self::$default_device_type = OpenCL::CL_DEVICE_TYPE_DEFAULT;
            $context = $ocl->Context(self::$default_device_type);
        }
        return $context;
    }

    public function newHostBuffer($size,$dtype)
    {
        return new HostBuffer($size,$dtype);
    }

    public function getCLBlastVersion($blas)
    {
        $config = $blas->getConfig();
        if(strpos($config,'OpenBLAS')===0) {
            $config = explode(' ',$config);
            return $config[1];
        } else {
            return '0.0.0';
        }
    }

    //
    //  scal
    //

    protected function translate_scal(
        $alpha,
        NDArray $X,
        $events=null
        ) : array
    {
        $N = $X->size();
        $XX = $X->buffer();
        $offX = $X->offset();
        $queue = $this->queue;
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }
        return [$N,$alpha,$XX,$offX,1,$queue,$events];
    }

    public function testScalNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $X = $this->array([1,2,3],$dtype);
        [$N,$alpha,$XX,$offX,$incX,$queue,$events] =
            $this->translate_scal(2,$X);

        $blas->scal($N,$alpha,$XX,$offX,$incX,$queue,$events);
        $events->wait();
        $this->assertEquals([2,4,6],$X->toArray());
    /*
        // complex64
        $dtype = NDArray::complex64;
        $X = $this->array($this->toComplex([1,2,3]),$dtype);
        [$N,$alpha,$XX,$offX,$incX,$queue,$events] =
            $this->translate_scal(C(2),$X);

        $blas->scal($N,$alpha,$XX,$offX,$incX,$queue,$events);
        $events->wait();
        $this->assertEquals($this->toComplex([2,4,6]),$X->toArray());
    */
        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $X = $this->array([1,2,3],$dtype);
            [$N,$alpha,$XX,$offX,$incX,$queue,$events] =
                $this->translate_scal(2,$X);

            $blas->scal($N,$alpha,$XX,$offX,$incX,$queue,$events);
            $events->wait();
            $this->assertEquals([2,4,6],$X->toArray());
    /*
            // complex128
            $dtype = NDArray::complex128;
            $X = $this->array($this->toComplex([1,2,3]),$dtype);
            [$N,$alpha,$XX,$offX,$incX,$queue,$events] =
                $this->translate_scal(C(2),$X);

            $blas->scal($N,$alpha,$XX,$offX,$incX,$queue,$events);
            $events->wait();
            $this->assertEquals($this->toComplex([2,4,6]),$X->toArray());
    */

        }
    }

    protected function getScalTestEnv(int $NMITEM) : array
    {
        $ocl = $this->getOpenCL();
        $queue = $this->queue;
        $context = $queue->getContext();
        $hostBuffer = $this->newHostBuffer($NMITEM,NDArray::float32);
        for($i=0;$i<$NMITEM;$i++) {
            $hostBuffer[$i]=$i;
        }
        $buffer = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBuffer);
        
        $blas = $this->getBlas();
        $events = $ocl->EventList();
        return [$queue,$blas,$events,$buffer,$hostBuffer];
    }

    public function testScalFullRange()
    {
        $blas = $this->getBlas();
        $NMITEM = 2048;
        $X = $this->array(range(0,$NMITEM-1),NDArray::float32);
        [$N,$alpha,$XX,$offX,$incX,$queue,$events] =
            $this->translate_scal(2,$X);
        $blas->scal($N,$alpha,$XX,$offX,$incX,$queue,$events);
        $events->wait();
        $trues = array_map(function($x) { return $x*2; }, range(0,$NMITEM-1));
        $this->assertEquals($trues,$X->toArray());
    }

    public function testScalOffsetRange()
    {
        $blas = $this->getBlas();
        $NMITEM = 2048;
        $X = $this->array(range(0,$NMITEM-1),NDArray::float32);
        [$N,$alpha,$XX,$offX,$incX,$queue,$events] =
            $this->translate_scal(2,$X);

        $blas->scal(intdiv($N,2),$alpha,$XX,intdiv($N,2),$incX,$queue,$events);
        $events->wait();
        $trues = array_map( function($x) use ($NMITEM) { return ($x>=intdiv($NMITEM,2))?$x*2:$x; } , range(0,$NMITEM-1));
        $this->assertEquals($trues,$X->toArray());
    }

    public function testScalLimitRange()
    {
        $NMITEM = 2048;
        [$queue,$blas,$events,$buffer,$hostBuffer] = $this->getScalTestEnv($NMITEM);
        // Limit-range
        $blas->scal(intval($NMITEM/2),$alpha=2.0,$buffer,
                    $offset=0,$inc=1,$queue,$events);
        $events->wait();
        $buffer->read($queue,$hostBuffer);
        $equals = true;
        for($i=0;$i<$NMITEM;$i++) {
            if($i<intval($NMITEM/2)) {
                if($hostBuffer[$i]!=$i*2) {
                    $equals = false;
                    break;
                }
            } else {
                if($hostBuffer[$i]!=$i) {
                    $equals = false;
                    break;
                }
            }
        }
        $this->assertTrue($equals);
    }

    public function testScalInvalidBufferObject()
    {
        $NMITEM = 2048;
        [$queue,$blas,$events,$buffer,$hostBuffer] = $this->getScalTestEnv($NMITEM);
        // invalid buffer object arguments
        $buffer = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->scal(intval(2),$alpha=1.0,
                $buffer,$offset=0,$inc=1,
                $queue,$events);
    }

    public function testScalInvalidQueueObject()
    {
        $NMITEM = 2048;
        [$queue,$blas,$events,$buffer,$hostBuffer] = $this->getScalTestEnv($NMITEM);
        // invalid queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->scal(intval(2),$alpha=1.0,
                $buffer,$offset=0,$inc=1,
                $queue,$events);
    }

    public function testScalInvalidEventObject()
    {
        $NMITEM = 2048;
        [$queue,$blas,$events,$buffer,$hostBuffer] = $this->getScalTestEnv($NMITEM);
        // invalid event object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->scal(intval(2),$alpha=1.0,
                $buffer,$offset=0,$inc=1,
                $queue,$events);
    }

    //
    //  axpy
    //

    protected function translate_axpy(
        NDArray $X,
        NDArray $Y,
        $alpha=null,
        $events=null) : array
    {
        if($X->shape()!=$Y->shape()) {
            $shapeError = '('.implode(',',$X->shape()).'),('.implode(',',$Y->shape()).')';
            throw new InvalidArgumentException("Unmatch shape of dimension: ".$shapeError);
        }
        $N = $X->size();
        $XX = $X->buffer();
        $offX = $X->offset();
        $YY = $Y->buffer();
        $offY = $Y->offset();
        if($alpha===null) {
            $alpha = $this->buildValByType(1.0,$X->dtype());
        }
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }
        return [$N,$alpha,$XX,$offX,1,$YY,$offY,1,$this->queue,$events];
    }

    public function testAxpyNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $x = $this->array([[1,2,3],[4,5,6]],$dtype);
        $y = $this->array([[10,20,30],[40,50,60]],$dtype);
        [$N,$alpha,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] = 
            $this->translate_axpy($x,$y,2);
        $blas->axpy($N,$alpha,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
        $events->wait();

        $this->assertEquals([[12,24,36],[48,60,72]],$y->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $x = $this->array($this->toComplex([[1,2,3],[4,5,6]]),$dtype);
        $y = $this->array($this->toComplex([[10,20,30],[40,50,60]]),$dtype);
        [$N,$alpha,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] = 
            $this->translate_axpy($x,$y,C(2));
        $blas->axpy($N,$alpha,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
        $events->wait();

        $this->assertEquals($this->toComplex([[12,24,36],[48,60,72]]),$y->toArray());
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $x = $this->array([[1,2,3],[4,5,6]],$dtype);
            $y = $this->array([[10,20,30],[40,50,60]],$dtype);
            [$N,$alpha,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] = 
                $this->translate_axpy($x,$y,2);
            $blas->axpy($N,$alpha,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
            $events->wait();

            $this->assertEquals([[12,24,36],[48,60,72]],$y->toArray());

            /*
            // complex128
            $dtype = NDArray::complex128;
            $x = $this->array($this->toComplex([[1,2,3],[4,5,6]]),$dtype);
            $y = $this->array($this->toComplex([[10,20,30],[40,50,60]]),$dtype);
            [$N,$alpha,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] = 
                $this->translate_axpy($x,$y,C(2));
            $blas->axpy($N,$alpha,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
            $events->wait();

            $this->assertEquals($this->toComplex([[12,24,36],[48,60,72]]),$y->toArray());
            */
        }
    }

    protected function getAxpyTestEnv($NMITEM)
    {
        $ocl = $this->getOpenCL();
        $queue = $this->queue;
        $context = $queue->getContext();
        $hostBufferX = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferY = $this->newHostBuffer($NMITEM,NDArray::float32);
        for($i=0;$i<$NMITEM;$i++) {
            $hostBufferX[$i]=$i;
            $hostBufferY[$i]=$NMITEM-1-$i;
        }
        $bufferX = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferY = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferY);
        $blas = $this->getBlas();
        $events = $ocl->EventList();
        return [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY];
    }

    public function testAxpyFullRange()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY] = $this->getAxpyTestEnv($NMITEM);
        $alpha=2.0;
        $blas->axpy($NMITEM,$alpha,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
        $events->wait();
        $bufferY->read($queue,$hostBufferY);
        $equals = true;
        for($i=0;$i<$NMITEM;$i++) {
            if($hostBufferY[$i]!=($i*2)+($NMITEM-1-$i)) {
                $equals = false;
                break;
            }
        }
        $this->assertTrue($equals);
    }

    public function testAxpyInvalidBufferXObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY] = $this->getAxpyTestEnv($NMITEM);
        $alpha=2.0;
        // invalid buffer object arguments
        $bufferX = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->axpy(intval(2),$alpha=1.0,
            $bufferX,$offset=0,$inc=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
    }

    public function testAxpyInvalidBufferYObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY] = $this->getAxpyTestEnv($NMITEM);
        $alpha=2.0;
        // invalid buffer object arguments
        $bufferY = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->axpy(intval(2),$alpha=1.0,
                $bufferX,$offset=0,$inc=1,
                $bufferY,$offsetY=0,$incY=1,
                $queue,$events);
    }

    public function testAxpyInvalidQueueObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY] = $this->getAxpyTestEnv($NMITEM);
        $alpha=2.0;
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->axpy(intval(2),$alpha=1.0,
                $bufferX,$offset=0,$inc=1,
                $bufferY,$offsetY=0,$incY=1,
                $queue,$events);
    }

    public function testAxpyInvalidEventsObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY] = $this->getAxpyTestEnv($NMITEM);
        $alpha=2.0;
        // invalid Events object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->axpy(intval(2),$alpha=1.0,
                $bufferX,$offset=0,$inc=1,
                $bufferY,$offsetY=0,$incY=1,
                $queue,$events);
    }

    //
    //  dot
    //

    protected function translate_dot(
        NDArray $X,
        NDArray $Y,
        NDArray $output=null,
        object $events=null) : array
    {
        $R = $output;
        if($X->shape()!=$Y->shape()) {
            $shapeError = '('.implode(',',$X->shape()).'),('.implode(',',$Y->shape()).')';
            throw new InvalidArgumentException("Unmatch shape of dimension: ".$shapeError);
        }
        if($R==null) {
            $R = $this->alloc([],$X->dtype(),OpenCL::CL_MEM_READ_WRITE);
        }
        $N = $X->size();
        $RR = $R->buffer();
        $offR = $R->offset();
        $XX = $X->buffer();
        $offX = $X->offset();
        $YY = $Y->buffer();
        $offY = $Y->offset();

        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }
        return [$N,$RR,$offR,$XX,$offX,1,$YY,$offY,1,$this->queue,$events];
    }

    public function testDotNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $x = $this->array([[1,2,3],[4,5,6]],$dtype);
        $y = $this->array([[10,20,30],[40,50,60]],$dtype);
        $ret = $this->zeros([],$dtype);
        [$N,$RR,$offR,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
            $this->translate_dot($x,$y,$ret);
        $blas->dot($N,$RR,$offR,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
        $events->wait();
        $this->assertEquals(1*10+2*20+3*30+4*40+5*50+6*60,$ret->toArray());

        /*
        // complex64 dotc
        $dtype = NDArray::complex64;
        $x = $this->array($this->toComplex([[1,2,3],[4,5,6]]),$dtype);
        $y = $this->array($this->toComplex([[10,20,30],[40,50,60]]),$dtype);
        $ret = $this->zeros([],$dtype);
        [$N,$RR,$offR,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
            $this->translate_dot($x,$y,$ret);
        $blas->dotc($N,$RR,$offR,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
        $events->wait();
        $this->assertEquals($this->toComplex(1*10+2*20+3*30+4*40+5*50+6*60),$ret->toArray());

        // complex64 dotu
        $dtype = NDArray::complex64;
        $x = $this->array($this->toComplex([[1,2,3],[4,5,6]]),$dtype);
        $y = $this->array($this->toComplex([[10,20,30],[40,50,60]]),$dtype);
        $ret = $this->zeros([],$dtype);
        [$N,$RR,$offR,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
            $this->translate_dot($x,$y,$ret);
        $blas->dotu($N,$RR,$offR,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
        $events->wait();
        $this->assertEquals($this->toComplex(1*10+2*20+3*30+4*40+5*50+6*60),$ret->toArray());
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $x = $this->array([[1,2,3],[4,5,6]],$dtype);
            $y = $this->array([[10,20,30],[40,50,60]],$dtype);
            $ret = $this->zeros([],$dtype);
            [$N,$RR,$offR,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
                $this->translate_dot($x,$y,$ret);
            $blas->dot($N,$RR,$offR,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
            $events->wait();
            $this->assertEquals(1*10+2*20+3*30+4*40+5*50+6*60,$ret->toArray());

            /*
            // complex128 dotc
            $dtype = NDArray::complex128;
            $x = $this->array($this->toComplex([[1,2,3],[4,5,6]]),$dtype);
            $y = $this->array($this->toComplex([[10,20,30],[40,50,60]]),$dtype);
            $ret = $this->zeros([],$dtype);
            [$N,$RR,$offR,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
                $this->translate_dot($x,$y,$ret);
            $blas->dotc($N,$RR,$offR,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
            $events->wait();
            $this->assertEquals($this->toComplex(1*10+2*20+3*30+4*40+5*50+6*60),$ret->toArray());

            // complex128 dotu
            $dtype = NDArray::complex128;
            $x = $this->array($this->toComplex([[1,2,3],[4,5,6]]),$dtype);
            $y = $this->array($this->toComplex([[10,20,30],[40,50,60]]),$dtype);
            $ret = $this->zeros([],$dtype);
            [$N,$RR,$offR,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
                $this->translate_dot($x,$y,$ret);
            $blas->dotu($N,$RR,$offR,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
            $events->wait();
            $this->assertEquals($this->toComplex(1*10+2*20+3*30+4*40+5*50+6*60),$ret->toArray());
            */

        }
    }

    protected function getDotTestEnv($NMITEM)
    {
        $ocl = $this->getOpenCL();
        $queue = $this->queue;
        $context = $queue->getContext();
        $hostBufferX = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferY = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferR = $this->newHostBuffer(1,NDArray::float32);
        $dot = 0.0;
        for($i=0;$i<$NMITEM;$i++) {
            $hostBufferX[$i]=$i/$NMITEM;
            $hostBufferY[$i]=($NMITEM-1-$i)/$NMITEM;
            $dot += $hostBufferX[$i]*$hostBufferY[$i];
        }
        $bufferX = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferY = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferY);
        $bufferR = $ocl->Buffer($context,intval(32/8),
            OpenCL::CL_MEM_WRITE_ONLY);
        $blas = $this->getBlas();
        $events = $ocl->EventList();
        return [$queue,$blas,$events,$bufferX,$bufferY,$bufferR,$hostBufferX,$hostBufferY,$hostBufferR,$dot];
    }

    public function testDotFullRange()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$bufferR,$hostBufferX,$hostBufferY,$hostBufferR,$dot]
            = $this->getDotTestEnv($NMITEM);
        $blas->dot(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
        $events->wait();
        $bufferR->read($queue,$hostBufferR);
        $this->assertTrue(abs($hostBufferR[0]-$dot)<1e-7);
    }

    public function testDotInvalidBufferRObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$bufferR,$hostBufferX,$hostBufferY,$hostBufferR,$dot]
            = $this->getDotTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferR = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->dot(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
    }

    public function testDotInvalidBufferXObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$bufferR,$hostBufferX,$hostBufferY,$hostBufferR,$dot]
            = $this->getDotTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferX = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->dot(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
    }

    public function testDotInvalidBufferYObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$bufferR,$hostBufferX,$hostBufferY,$hostBufferR,$dot]
            = $this->getDotTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferY = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->dot(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
    }

    public function testDotInvalidQueueObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$bufferR,$hostBufferX,$hostBufferY,$hostBufferR,$dot]
            = $this->getDotTestEnv($NMITEM);
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->dot(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
    }

    public function testDotInvalidEventsObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$bufferR,$hostBufferX,$hostBufferY,$hostBufferR,$dot]
            = $this->getDotTestEnv($NMITEM);
        // invalid Events object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->dot(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
    }

    //
    //  asum
    //

    protected function translate_asum(
        NDArray $X,
        NDArray $output=null,
        object $events=null) : array
    {
        $R = $output;
        if($R==null) {
            throw new \Exception('output buffer must be set');
        }
        $N = $X->size();
        $RR = $R->buffer();
        $offR = $R->offset();
        $XX = $X->buffer();
        $offX = $X->offset();
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }
        return [$N,$RR,$offR,$XX,$offX,1,$this->queue,$events];
    }

    public function testAsumNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $outdtype = NDArray::float32;
        $x = $this->array([[-1,2,-3],[-4,5,-6]],$dtype);
        $ret = $this->zeros([],$outdtype);
        [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
            $this->translate_asum($x,$ret);
        $blas->asum($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
        $events->wait();
        $this->assertEquals(1+2+3+4+5+6,$ret->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $outdtype = NDArray::complex64; // **** CAUTION ****
        $x = $this->array($this->toComplex([[-1,2,-3],[-4,5,-6]]),$dtype);
        $ret = $this->zeros([],$outdtype);
        [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
            $this->translate_asum($x,$ret);
        $blas->asum($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
        $events->wait();
        $this->assertEquals($this->toComplex(1+2+3+4+5+6),$ret->toArray());
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $outdtype = NDArray::float64;
            $x = $this->array([[-1,2,-3],[-4,5,-6]],$dtype);
            $ret = $this->zeros([],$outdtype);
            [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
                $this->translate_asum($x,$ret);
            $blas->asum($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
            $events->wait();
            $this->assertEquals(1+2+3+4+5+6,$ret->toArray());

            /*
            // complex128
            $dtype = NDArray::complex128;
            $outdtype = NDArray::complex128; // **** CAUTION ****
            $x = $this->array($this->toComplex([[-1,2,-3],[-4,5,-6]]),$dtype);
            $ret = $this->zeros([],$outdtype);
            [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
                $this->translate_asum($x,$ret);
            $blas->asum($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
            $events->wait();
            $this->assertEquals($this->toComplex(1+2+3+4+5+6),$ret->toArray());
            */
        }

    }

    protected function getAsumTestEnv($NMITEM)
    {
        $ocl = $this->getOpenCL();
        $queue = $this->queue;
        $context = $queue->getContext();
        $hostBufferX = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferR = $this->newHostBuffer(1,NDArray::float32);
        $asum = 0.0;
        for($i=0;$i<$NMITEM;$i++) {
            $hostBufferX[$i]=$i/$NMITEM;
            $asum += abs($hostBufferX[$i]);
        }
        $bufferX = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferR = $ocl->Buffer($context,intval(32/8),
            OpenCL::CL_MEM_WRITE_ONLY);
        $blas = $this->getBlas();
        $events = $ocl->EventList();
        return [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$asum];
    }

    public function testAsumFullRange()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$asum]
            = $this->getAsumTestEnv($NMITEM);
        $blas->asum(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
        $events->wait();
        $bufferR->read($queue,$hostBufferR);
        $this->assertTrue(abs($hostBufferR[0]-$asum)<1e-7);
    }

    public function testAsumInvalidBufferRObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$asum]
            = $this->getAsumTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferR = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->asum(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testAsumInvalidBufferXObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$asum]
            = $this->getAsumTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferX = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->asum(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testAsumInvalidQueueObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$asum]
            = $this->getAsumTestEnv($NMITEM);
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->asum(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testAsumInvalidEventsObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$asum]
            = $this->getAsumTestEnv($NMITEM);
        // invalid Events object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->asum(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    //
    //  iamax
    //

    public function testIamaxNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $x = $this->array([[-1,2,-3],[-4,5,-6]],$dtype);
        $ret = $this->zeros([],NDArray::int32);
        [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
            $this->translate_iamin($x,$ret);

        $blas->iamax($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
        $events->wait();
        $this->assertEquals(5,$ret->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $x = $this->array($this->toComplex([[-1,2,-3],[-4,5,-6]]),$dtype);
        $ret = $this->zeros([],NDArray::int32);
        [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
            $this->translate_iamin($x,$ret);

        $blas->iamax($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
        $events->wait();
        $this->assertEquals(5,$ret->toArray());
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $x = $this->array([[-1,2,-3],[-4,5,-6]],$dtype);
            $ret = $this->zeros([],NDArray::int32);
            [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
                $this->translate_iamin($x,$ret);

            $blas->iamax($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
            $events->wait();
            $this->assertEquals(5,$ret->toArray());

            /*
            // complex128
            $dtype = NDArray::complex128;
            $x = $this->array($this->toComplex([[-1,2,-3],[-4,5,-6]]),$dtype);
            $ret = $this->zeros([],NDArray::int32);
            [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
                $this->translate_iamin($x,$ret);

            $blas->iamax($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
            $events->wait();
            $this->assertEquals(5,$ret->toArray());
            */
        }
    }

    protected function getIamaxTestEnv($NMITEM)
    {
        $ocl = $this->getOpenCL();
        $queue = $this->queue;
        $context = $queue->getContext();
        $hostBufferX = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferR = $this->newHostBuffer(1,NDArray::int32);
        $amax = 0.0;
        $iamax = -1;
        for($i=0;$i<$NMITEM;$i++) {
            $hostBufferX[$i]=$i/$NMITEM;
            if(abs($hostBufferX[$i])>$amax) {
                $amax = abs($hostBufferX[$i]);
                $iamax = $i;
            }
        }
        $bufferX = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferR = $ocl->Buffer($context,intval(32/8),
            OpenCL::CL_MEM_WRITE_ONLY);
        $blas = $this->getBlas();
        $events = $ocl->EventList();
        return [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$iamax];
    }

    public function testIamaxFullRange()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$iamax]
            = $this->getIamaxTestEnv($NMITEM);
        $blas->iamax(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
        $events->wait();
        $bufferR->read($queue,$hostBufferR);
        $this->assertTrue(abs($hostBufferR[0]-$iamax)<1e-7);
    }

    public function testIamaxInvalidBufferRObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$iamax]
            = $this->getIamaxTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferR = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->iamax(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testIamaxInvalidBufferXObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$iamax]
            = $this->getIamaxTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferX = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->iamax(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testIamaxInvalidQueueObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$iamax]
            = $this->getIamaxTestEnv($NMITEM);
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->iamax(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testIamaxInvalidEventsObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$iamax]
            = $this->getIamaxTestEnv($NMITEM);
        // invalid Queue object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->iamax(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    //
    //  iamin
    //

    protected function translate_iamin(
        NDArray $X,
        NDArray $output=null,
        object $events=null)
    {
        $R = $output;
        if($R==null) {
            // *** CAUTION ****
            // Index result is 32bit
            $R = $this->alloc([],NDArray::int32,OpenCL::CL_MEM_READ_WRITE);
        }
        $N = $X->size();
        $RR = $R->buffer();
        $offR = $R->offset();
        $XX = $X->buffer();
        $offX = $X->offset();
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }
        return [$N,$RR,$offR,$XX,$offX,1,$this->queue,$events];
    }

    protected function translate_amin(
        NDArray $X,
        NDArray $output=null,
        object $events=null)
    {
        $R = $output;
        if($R==null) {
            // *** CAUTION ****
            // Index result is 32bit
            $R = $this->alloc([],$X->dtype(),OpenCL::CL_MEM_READ_WRITE);
        }
        $N = $X->size();
        $RR = $R->buffer();
        $offR = $R->offset();
        $XX = $X->buffer();
        $offX = $X->offset();
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }
        return [$N,$RR,$offR,$XX,$offX,1,$this->queue,$events];
    }

    public function testIaminNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $x = $this->array([[-1,2,-3],[-4,5,-6]],$dtype);
        $ret = $this->zeros([],NDArray::int32);
        [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
            $this->translate_iamin($x,$ret);

        $blas->iamin($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
        $events->wait();
        $this->assertEquals(0,$ret->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $x = $this->array($this->toComplex([[-1,2,-3],[-4,5,-6]]),$dtype);
        $ret = $this->zeros([],NDArray::int32);
        [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
            $this->translate_iamin($x,$ret);

        $blas->iamin($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
        $events->wait();
        $this->assertEquals(0,$ret->toArray());
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $x = $this->array([[-1,2,-3],[-4,5,-6]],$dtype);
            $ret = $this->zeros([],NDArray::int32);
            [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
                $this->translate_iamin($x,$ret);

            $blas->iamin($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
            $events->wait();
            $this->assertEquals(0,$ret->toArray());

            /*
            // complex128
            $dtype = NDArray::complex128;
            $x = $this->array($this->toComplex([[-1,2,-3],[-4,5,-6]]),$dtype);
            $ret = $this->zeros([],NDArray::int32);
            [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
                $this->translate_iamin($x,$ret);

            $blas->iamin($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
            $events->wait();
            $this->assertEquals(0,$ret->toArray());
            */
        }
    }

    protected function getIaminTestEnv($NMITEM)
    {
        $ocl = $this->getOpenCL();
        $queue = $this->queue;
        $context = $queue->getContext();
        $hostBufferX = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferR = $this->newHostBuffer(1,NDArray::int32);
        $amin = 100000000;
        $iamin = -1;
        for($i=0;$i<$NMITEM;$i++) {
            $hostBufferX[$i]=$i/$NMITEM;
            if(abs($hostBufferX[$i])<$amin) {
                $amin = abs($hostBufferX[$i]);
                $iamin = $i;
            }
        }
        $bufferX = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferR = $ocl->Buffer($context,intval(32/8),
            OpenCL::CL_MEM_WRITE_ONLY);
        $blas = $this->getBlas();
        $events = $ocl->EventList();
        return [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$iamin];
    }

    public function testIaminFullRange()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$iamin]
            = $this->getIaminTestEnv($NMITEM);
        $blas->iamin(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
        $events->wait();
        $bufferR->read($queue,$hostBufferR);
        $this->assertTrue(abs($hostBufferR[0]-$iamin)<1e-7);
    }

    public function testIaminInvalidBufferRObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$iamin]
            = $this->getIaminTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferR = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->iamin(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testIaminInvalidBufferXObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$iamin]
            = $this->getIaminTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferX = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->iamin(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testIaminInvalidQueueObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$iamin]
            = $this->getIaminTestEnv($NMITEM);
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->iamin(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testIaminInvalidEventsObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$iamin]
            = $this->getIaminTestEnv($NMITEM);
        // invalid Events object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->iamin(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    //
    //  copy
    //

    protected function translate_copy(
        NDArray $X,
        NDArray $output=null,
        object $events=null) : array
    {
        $Y = $output;
        if($Y===null) {
            $Y = $this->alloc($X->shape(),$X->dtype(),OpenCL::CL_MEM_READ_WRITE);
        } else {
            if($X->shape()!=$Y->shape()) {
                $shapeError = '('.implode(',',$X->shape()).'),('.implode(',',$Y->shape()).')';
                throw new InvalidArgumentException("Unmatch shape of dimension: ".$shapeError);
            }
        }
        $N = $X->size();
        $XX = $X->buffer();
        $offX = $X->offset();
        $YY = $Y->buffer();
        $offY = $Y->offset();
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }
        return [$N,$XX,$offX,1,$YY,$offY,1,$this->queue,$events];
    }

    public function testCopyNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $x = $this->array([[-1,2,-3],[-4,5,-6]],$dtype);
        $y = $this->zerosLike($x);
        [$N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
            $this->translate_copy($x,$y);
        $blas->copy($N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
        $events->wait();

        $this->assertEquals([[-1,2,-3],[-4,5,-6]],$y->toArray());

        // int32
        $dtype = NDArray::int32;
        $x = $this->array([[-1,2,-3],[-4,5,-6]],$dtype);
        $y = $this->zerosLike($x);
        [$N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
            $this->translate_copy($x,$y);
        $blas->copy($N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
        $events->wait();

        $this->assertEquals([[-1,2,-3],[-4,5,-6]],$y->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $x = $this->array($this->toComplex([[-1,2,-3],[-4,5,-6]]),$dtype);
        $y = $this->zerosLike($x);
        [$N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
            $this->translate_copy($x,$y);
        $blas->copy($N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
        $events->wait();

        $this->assertEquals($this->toComplex([[-1,2,-3],[-4,5,-6]]),$y->toArray());
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $x = $this->array([[-1,2,-3],[-4,5,-6]],$dtype);
            $y = $this->zerosLike($x);
            [$N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
                $this->translate_copy($x,$y);
            $blas->copy($N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
            $events->wait();

            $this->assertEquals([[-1,2,-3],[-4,5,-6]],$y->toArray());

            /*
            // complex128
            $dtype = NDArray::complex128;
            $x = $this->array($this->toComplex([[-1,2,-3],[-4,5,-6]]),$dtype);
            $y = $this->zerosLike($x);
            [$N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
                $this->translate_copy($x,$y);
            $blas->copy($N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
            $events->wait();

            $this->assertEquals($this->toComplex([[-1,2,-3],[-4,5,-6]]),$y->toArray());
            */
        }
    }

    protected function getCopyTestEnv($NMITEM,$dtype)
    {
        $ocl = $this->getOpenCL();
        $queue = $this->queue;
        $context = $queue->getContext();
        $hostBufferX = $this->newHostBuffer($NMITEM,$dtype);
        $hostBufferY = $this->newHostBuffer($NMITEM,$dtype);
        for($i=0;$i<$NMITEM;$i++) {
            $hostBufferX[$i]=$i;
            $hostBufferY[$i]=$NMITEM-1-$i;
        }
        $bufferX = $ocl->Buffer($context,intval($NMITEM*$hostBufferX->value_size()),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferY = $ocl->Buffer($context,intval($NMITEM*$hostBufferY->value_size()),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferY);
        $blas = $this->getBlas();
        $events = $ocl->EventList();
        return [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY];
    }

    public function testCopyFloat32()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY]
            = $this->getCopyTestEnv($NMITEM,NDArray::float32);
        $blas->copy(
            $NMITEM,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
        $events->wait();
        $bufferY->read($queue,$hostBufferY);
        $equals = true;
        for($i=0;$i<$NMITEM;$i++) {
            if($hostBufferY[$i]!=$i) {
                $equals = false;
                break;
            }
        }
        $this->assertTrue($equals);
    }

    public function testCopyInt8()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY]
            = $this->getCopyTestEnv($NMITEM,NDArray::int8);
        $blas->copy(
            $NMITEM,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
        $events->wait();
        $bufferY->read($queue,$hostBufferY);
        $equals = true;
        for($i=0;$i<$NMITEM;$i++) {
            if($hostBufferY[$i]!=(($i+128)%256)-128 ) {
                $equals = false;
                break;
            }
        }
        $this->assertTrue($equals);
    }

    public function testCopyInt64()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY]
            = $this->getCopyTestEnv($NMITEM,NDArray::int64);
        $blas->copy(
            $NMITEM,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
        $events->wait();
        $bufferY->read($queue,$hostBufferY);
        $equals = true;
        for($i=0;$i<$NMITEM;$i++) {
            if($hostBufferY[$i]!=$i) {
                $equals = false;
                break;
            }
        }
        $this->assertTrue($equals);
    }

    public function testCopyInvalidBufferXObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY]
            = $this->getCopyTestEnv($NMITEM,NDArray::float32);
        // invalid Buffer object arguments
        $bufferX = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->copy(
            $NMITEM,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
    }

    public function testCopyInvalidBufferYObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY]
            = $this->getCopyTestEnv($NMITEM,NDArray::float32);
        // invalid Buffer object arguments
        $bufferY = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->copy(
            $NMITEM,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
    }

    public function testCopyInvalidQueueObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY]
            = $this->getCopyTestEnv($NMITEM,NDArray::float32);
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->copy(
            $NMITEM,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
    }

    public function testCopyInvalidEventsObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY]
            = $this->getCopyTestEnv($NMITEM,NDArray::float32);
        // invalid Events object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->copy(
            $NMITEM,
            $bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events
        );
    }

    //
    //  nrm2
    //

    public function testNrm2Normal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $X = $this->array([[1,2],[3,4]],$dtype);
        $R = $this->zeros([],$dtype);
        [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
            $this->translate_amin($X,$R);

        $blas->nrm2($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
        $events->wait();
        $true = sqrt(1+2**2+3**2+4**2);
        $nrm2 = $R->toArray();
        $this->assertLessThan(0.00001,abs($nrm2-$true));

        /*
        // complex64
        $dtype = NDArray::complex64;
        $X = $this->array($this->toComplex([[1,2],[3,4]]),$dtype);
        $R = $this->zeros([],$dtype);
        [$N,$RR,$offR,$XX,$offX,$incX,$queue,$events] =
            $this->translate_amin($X,$R);

        $blas->nrm2($N,$RR,$offR,$XX,$offX,$incX,$queue,$events);
        $events->wait();
        $true = sqrt(1+2**2+3**2+4**2);
        $nrm2 = $R->toArray()->real;
        $this->assertLessThan(0.00001,abs($nrm2-$true));
        */

    }

    protected function getNrm2TestEnv($NMITEM)
    {
        $ocl = $this->getOpenCL();
        $queue = $this->queue;
        $context = $queue->getContext();
        $hostBufferX = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferR = $this->newHostBuffer(1,NDArray::float32);
        $nrm2 = 0.0;
        for($i=0;$i<$NMITEM;$i++) {
            $hostBufferX[$i]=$i/$NMITEM;
        }
        $nrm2 = $this->getOpenBLAS()->nrm2($NMITEM,$hostBufferX, 0, 1);
        $bufferX = $ocl->Buffer($context,intval($NMITEM*$hostBufferX->value_size()),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferR = $ocl->Buffer($context,$hostBufferR->value_size(),
            OpenCL::CL_MEM_WRITE_ONLY);
        $blas = $this->getBlas();
        $events = $ocl->EventList();
        return [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$nrm2];
    }

    public function testNrm2FullRange()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$nrm2]
            = $this->getNrm2TestEnv($NMITEM);
        $blas->nrm2($NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events);
        $events->wait();
        $bufferR->read($queue,$hostBufferR);
        $this->assertTrue(abs($hostBufferR[0]-$nrm2)<1e-7);
    }

    public function testNrm2InvalidBufferRObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$nrm2]
            = $this->getNrm2TestEnv($NMITEM);
        // invalid bufferR object arguments
        $bufferR = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->nrm2($NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events);
    }

    public function testNrm2InvalidBufferXObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$nrm2]
            = $this->getNrm2TestEnv($NMITEM);
        // invalid buffer object arguments
        $bufferX = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->nrm2($NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events);
    }

    public function testNrm2InvalidQueueObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$nrm2]
            = $this->getNrm2TestEnv($NMITEM);
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->nrm2($NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events);
    }

    public function testNrm2InvalidEventsObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$nrm2]
            = $this->getNrm2TestEnv($NMITEM);
        // invalid Events object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->nrm2($NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events);
    }

    //
    //  swap
    //

    public function testSwapNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $X = $this->array([100,10,1],$dtype);
        $Y = $this->array([200,20,2],$dtype);
        [$N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
            $this->translate_copy($X,$Y);

        $blas->swap($N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
        $events->wait();
        $this->assertEquals([200,20,2],$X->toArray());
        $this->assertEquals([100,10,1],$Y->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $X = $this->array([C(100,i:1),C(10,i:10),C(1,i:100)],$dtype);
        $Y = $this->array([C(200),C(20),C(2)],$dtype);
        [$N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
            $this->translate_copy($X,$Y);

        $blas->swap($N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
        $events->wait();
        $this->assertEquals($this->toComplex([200,20,2]),$this->toComplex($X->toArray()));
        $this->assertEquals($this->toComplex([C(100,i:1),C(10,i:10),C(1,i:100)]),$this->toComplex($Y->toArray()));
        */

        if($this->fp64()) {
            // float32
            $dtype = NDArray::float32;
            $X = $this->array([100,10,1],$dtype);
            $Y = $this->array([200,20,2],$dtype);
            [$N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
                $this->translate_copy($X,$Y);
            
            $blas->swap($N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
            $events->wait();
            $this->assertEquals([200,20,2],$X->toArray());
            $this->assertEquals([100,10,1],$Y->toArray());
            
            /*
            // complex64
            $dtype = NDArray::complex64;
            $X = $this->array([C(100,i:1),C(10,i:10),C(1,i:100)],$dtype);
            $Y = $this->array([C(200),C(20),C(2)],$dtype);
            [$N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] =
                $this->translate_copy($X,$Y);
            
            $blas->swap($N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
            $events->wait();
            $this->assertEquals($this->toComplex([200,20,2]),$this->toComplex($X->toArray()));
            $this->assertEquals($this->toComplex([C(100,i:1),C(10,i:10),C(1,i:100)]),$this->toComplex($Y->toArray()));
            */
        }

    }

    protected function getSwapTestEnv($NMITEM)
    {
        $ocl = $this->getOpenCL();
        $queue = $this->queue;
        $context = $queue->getContext();
        $hostBufferX = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferY = $this->newHostBuffer($NMITEM,NDArray::float32);
        for($i=0;$i<$NMITEM;$i++) {
            $hostBufferX[$i] = $i;
            $hostBufferY[$i] = $NMITEM-$i;
        }
        $bufferX = $ocl->Buffer($context,intval($NMITEM*$hostBufferX->value_size()),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferY = $ocl->Buffer($context,intval($NMITEM*$hostBufferY->value_size()),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferY);
        $blas = $this->getBlas();
        $events = $ocl->EventList();
        return [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY];
    }

    public function testSwapFullRange()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY]
            = $this->getSwapTestEnv($NMITEM,NDArray::float32);
        $blas->swap(
            $NMITEM,
            $bufferX,$offsetA=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
        $events->wait();
        $bufferX->read($queue,$hostBufferX);
        $bufferY->read($queue,$hostBufferY);
        $equals = true;
        for($i=0;$i<$NMITEM;$i++) {
            if($hostBufferX[$i] != $NMITEM-$i) {
                $equals = false;
                break;
            }
            if($hostBufferY[$i] != $i) {
                $equals = false;
                break;
            }
        }
        $this->assertTrue($equals);
    }

    public function testSwapInvalidBufferXObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY]
            = $this->getSwapTestEnv($NMITEM,NDArray::float32);
        // invalid buffer object arguments
        $bufferX = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->swap(
            $NMITEM,
            $bufferX,$offsetA=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
    }

    public function testSwapInvalidBufferYObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY]
            = $this->getSwapTestEnv($NMITEM,NDArray::float32);
        // invalid buffer object arguments
        $bufferY = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->swap(
            $NMITEM,
            $bufferX,$offsetA=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
    }

    public function testSwapInvalidQueueObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY]
            = $this->getSwapTestEnv($NMITEM,NDArray::float32);
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->swap(
            $NMITEM,
            $bufferX,$offsetA=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
    }

    public function testSwapInvalidEventsObject()
    {
        $NMITEM = 1024;
        [$queue,$blas,$events,$bufferX,$bufferY,$hostBufferX,$hostBufferY]
            = $this->getSwapTestEnv($NMITEM,NDArray::float32);
        // invalid Events object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->swap(
            $NMITEM,
            $bufferX,$offsetA=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
    }

    //
    //  rotg
    //

    public function translate_rotg(
        NDArray $X,
        NDArray $Y,
        NDArray $R=null,
        NDArray $Z=null,
        NDArray $C=null,
        NDArray $S=null,
        object $events=null
        ) : array
    {
        if($X->shape()!=$Y->shape()) {
            $shapeError = '('.implode(',',$X->shape()).'),('.implode(',',$Y->shape()).')';
            throw new InvalidArgumentException("Unmatch shape of dimension: ".$shapeError);
        }
        $R = $this->copy($X,$R);
        $Z = $this->copy($Y,$Z);
        if($C==null) {
            $C = $this->alloc($X->shape(),$X->dtype());
        }
        if($S==null) {
            $S = $this->alloc($Y->shape(),$X->dtype());
        }
        $AA = $R->buffer();
        $offA = $R->offset();
        $BB = $Z->buffer();
        $offB = $Z->offset();
        $CC = $C->buffer();
        $offC = $C->offset();
        $SS = $S->buffer();
        $offS = $S->offset();
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }
        return [
            $AA,$offA,
            $BB,$offB,
            $CC,$offC,
            $SS,$offS,
            $this->queue,$events
        ];
    }

    /**
     * NOT Implemented in CLBlast library
     */
    public function testRotgNormal()
    {
        $this->markTestSkipped('NOT Implemented in CLBlast library.');
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $inputs = [
            [1,1],
            [2,2],
            [3,3],
            [4,4],
            [5,5],
        ];
        foreach($inputs as [$xx,$yy]) {
            $X = $this->array($xx,$dtype);
            $Y = $this->array($yy,$dtype);
            [$AA,$offA,$BB,$offB,$CC,$offC,$SS,$offS,$queue,$events] =
                $this->translate_rotg($X,$Y);
            $blas->rotg($AA,$offA,$BB,$offB,$CC,$offC,$SS,$offS,$queue,$events);
            $events->wait();

            $rr = $AA[0];
            $zz = $BB[0];
            $cc = $CC[0];
            $ss = $SS[0];
            //echo "(x,y)=(".$X->buffer()[0].", ".$Y->buffer()[0].")\n";
            //echo "(r,z)=(".$rr.", ".$zz.")\n";
            //echo "(c,s)=(".$cc.", ".$ss.")\n";
            $this->assertLessThan(1e-7,abs($xx-$X->buffer()[0]));
            $this->assertLessThan(1e-7,abs($yy-$Y->buffer()[0]));
            $rx =  $cc * $xx + $ss * $yy;
            $ry = -$ss * $xx + $cc * $yy;
            #echo "(rx,ry)=(".$rx.",".$ry.")\n";
            $this->assertLessThan(1e-6,abs($rr-$rx));
            $this->assertLessThan(1e-6,abs(0-$ry));
        }

    }
   
    //
    //  rot
    //

    public function translate_rot(
        NDArray $X,
        NDArray $Y,
        float $cos,
        float $sin,
        object $events=null
        ) : array
    {
        if($X->shape()!=$Y->shape()) {
            $shapeError = '('.implode(',',$X->shape()).'),('.implode(',',$Y->shape()).')';
            throw new InvalidArgumentException("Unmatch shape of dimension: ".$shapeError);
        }
        $N = $X->size();
        $XX = $X->buffer();
        $offX = $X->offset();
        $YY = $Y->buffer();
        $offY = $Y->offset();
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }

        return [ $N,
            $XX,$offX,1,$YY,$offY,1,
            $cos,$sin,
            $this->queue,$events
        ];
    }

    /**
     * NOT Implemented in CLBlast library
     */
    public function testRotNormal()
    {
        $this->markTestSkipped('NOT Implemented in CLBlast library.');
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $x = $this->array([1,2,3,4,5],$dtype);
        $y = $this->array([1,2,3,4,5],$dtype);
        $c = cos(pi()/4);
        $s = sin(pi()/4);

        [
            $N,
            $XX,$offX,$incX,$YY,$offY,$incY,
            $cos,$sin,$queue,$events
        ] = $this->translate_rot($x,$y,$c,$s);

        $blas->rot(
            $N,
            $XX,$offX,$incX,$YY,$offY,$incY,
            $cos,$sin,$queue,$events
        );
        for($i=0;$i<5;$i++) {
            $this->assertLessThan(1e-6,abs(sqrt(2)*($i+1)-$x->buffer()[$i]));
            $this->assertLessThan(1e-6,abs($y->buffer()[$i]));
        }
        [
            $N,
            $XX,$offX,$incX,$YY,$offY,$incY,
            $cos,$sin,$queue,$events
        ] = $this->translate_rot($x,$y,$c,$s);

        $blas->rot(
            $N,
            $XX,$offX,$incX,$YY,$offY,$incY,
            $cos,$sin,$queue,$events
        );
        for($i=0;$i<5;$i++) {
            $this->assertLessThan(1e-6,abs(($i+1)-$x->buffer()[$i]));
            $this->assertLessThan(1e-6,abs((-$i-1)-$y->buffer()[$i]));
        }
    }

    /**
     * d1,d2,b1,p = rotmg(x,y)   b1: rotated x   p: params  d1,d2:works
     */
    public function translate_rotmg(
        NDArray $X,
        NDArray $Y,
        NDArray $D1=null,
        NDArray $D2=null,
        NDArray $B1=null,
        NDArray $B2=null,
        NDArray $P=null,
        object $events=null) : array
    {
        if($X->size()!=1||$Y->size()!=1) {
            $shapeError = '('.implode(',',$X->shape()).'),('.implode(',',$Y->shape()).')';
            throw new InvalidArgumentException("Unmatch shape of dimension: ".$shapeError);
        }
        if($D1==null) {
            $D1 = $this->ones([],$X->dtype());
        }
        if($D2==null) {
            $D2 = $this->ones([],$X->dtype());
        }
        if($B1==null) {
            $B1 = $this->alloc([],$X->dtype());
        }
        $B2 = $this->alloc([],$Y->dtype());
        if($P==null) {
            $P = $this->zeros([5],$X->dtype());
        }
        $this->copy($X->reshape([1]),$B1->reshape([1]));
        $this->copy($X->reshape([1]),$B2->reshape([1]));

        $DD1 = $D1->buffer();
        $offD1 = $D1->offset();
        $DD2 = $D2->buffer();
        $offD2 = $D2->offset();
        $BB1 = $B1->buffer();
        $offB1 = $B1->offset();
        $BB2 = $B2->buffer();
        $offB2 = $B2->offset();
        $PP = $P->buffer();
        $offP = $P->offset();
        return [
            $DD1,$offD1,
            $DD2,$offD2,
            $BB1,$offB1,
            $BB2,$offB2,
            $PP,$offP,
            $this->queue,$events,
        ];
    }

    /**
     * NOT Implemented in CLBlast library
     */
    public function testRotmgNormal()
    {
        $this->markTestSkipped('NOT Implemented in CLBlast library.');
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $inputs = [
            [1,1],
            [2,2],
            [3,3],
            [4,4],
            [5,5],
        ];
        foreach($inputs as [$xx,$yy]) {
            $X = $this->array($xx,$dtype);
            $Y = $this->array($yy,$dtype);
            $D1 = $this->ones([],$dtype);
            $D2 = $this->ones([],$dtype);
            $B1 = $this->zeros([],$dtype);
            $B2 = $this->zeros([],$dtype);
            $P = $this->zeros([5],$dtype);
            [
                $DD1,$offD1,
                $DD2,$offD2,
                $BB1,$offB1,
                $BB2,$offB2,
                $PP,$offP,
                $queue,$events,
            ] = $this->translate_rotmg($X,$Y,$D1,$D2,$B1,$B2,$P);
            $blas->rotmg(
                $DD1,$offD1,
                $DD2,$offD2,
                $BB1,$offB1,
                $BB2,$offB2,
                $PP,$offP,
                $queue,$events
            );
            $events->wait();

            $d1x = $D1->toArray();
            $d2y = $D2->toArray();
            $b1  = $B1->toArray();
            //echo "(x,y)=(".$X->buffer()[0].", ".$Y->buffer()[0].")\n";
            //echo "(r,z)=(".$rr.", ".$zz.")\n";
            //echo "(c,s)=(".$cc.", ".$ss.")\n";
            #echo "(rx,ry)=(".$rx.",".$ry.")\n";
            $this->assertTrue(true);
        }

    }

    /**
    *    x,y := rot(x,y,p)
    */
    public function translate_rotm(
        NDArray $X,
        NDArray $Y,
        NDArray $P,
        object $events=null) : array
    {
        if($X->shape()!=$Y->shape()) {
            $shapeError = '('.implode(',',$X->shape()).'),('.implode(',',$Y->shape()).')';
            throw new InvalidArgumentException("Unmatch shape of dimension: ".$shapeError);
        }
        $N = $X->size();
        $XX = $X->buffer();
        $offX = $X->offset();
        $YY = $Y->buffer();
        $offY = $Y->offset();
        $PP = $P->buffer();
        $offP = $P->offset();
        return [
            $N,
            $XX,$offX,1,
            $YY,$offY,1,
            $PP,$offP,
            $this->queue,$events,
        ];
    }

    /**
     * NOT Implemented in CLBlast library
     */
    public function testRotmNormal()
    {
        $this->markTestSkipped('NOT Implemented in CLBlast library.');
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $x = $this->array([1,2,3,4,5],$dtype);
        $y = $this->array([1,2,3,4,5],$dtype);
        $p = $this->array([-2,0,0,0,0]);

        [
            $N,
            $XX,$offX,$incX,
            $YY,$offY,$incY,
            $PP,$offP,
            $queue,$events,
        ] = $this->translate_rotm($x,$y,$p);

        $blas->rotm(
            $N,
            $XX,$offX,$incX,
            $YY,$offY,$incY,
            $PP,$offP,
            $queue,$events
        );
        $this->assertTrue(true);
    }

    //
    //  gemv
    //

    protected function translate_gemv(
        NDArray $A,
        NDArray $X,
        float $alpha=null,
        float $beta=null,
        NDArray $Y=null,
        bool $trans=null,
        bool $conj=null,
        object $events=null
        )
    {
        [$trans,$conj] = $this->complementTrans($trans,$conj,$A->dtype());

        if($A->ndim()!=2 || $X->ndim()!=1) {
            throw new InvalidArgumentException('"A" must be 2D-NDArray and "X" must 1D-NDArray.');
        }
        $shapeA = $A->shape();
        $shapeX = $X->shape();
        $rows = (!$trans) ? $shapeA[0] : $shapeA[1];
        $cols = (!$trans) ? $shapeA[1] : $shapeA[0];
        if($cols!=$shapeX[0]) {
            throw new InvalidArgumentException('The number of columns in "A" and The number of item in "X" must be the same');
        }
        $AA = $A->buffer();
        $XX = $X->buffer();
        $offA = $A->offset();
        $offX = $X->offset();
        $m = $shapeA[0];
        $n = $shapeA[1];
        if($alpha===null) {
            $alpha = $this->buildValByType(1.0,$A->dtype());
        }
        if($beta===null) {
            $beta = $this->buildValByType(0.0,$A->dtype());
        }
        if($Y!=null) {
            if($Y->ndim()!=1) {
                throw new InvalidArgumentException('"Y" must 1D-NDArray.');
            }
            $shapeY = $Y->shape();
            if($rows!=$shapeY[0]) {
                throw new InvalidArgumentException('The number of rows in "A" and The number of item in "Y" must be the same');
            }
        } else {
            $Y = $this->mo->zeros([$rows]);
        }
        $YY = $Y->buffer();
        $offY = $Y->offset();
        $trans = $this->transToCode($trans,$conj);
        $order = BLAS::RowMajor;

        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }
        return [
            $order,$trans,
            $m,$n,
            $alpha,
            $AA,$offA,$n,
            $XX,$offX,1,
            $beta,
            $YY,$offY,1,
            $this->queue,$events,
        ];
    }

    public function testGemvNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $A = $this->array([[1,2,3],[4,5,6]],$dtype);
        $X = $this->array([100,10,1],$dtype);
        $Y = $this->ones([2],$dtype);

        [ $order,$trans,$m,$n,$alpha,$AA,$offA,$ldA,
          $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
            $this->translate_gemv($A,$X,null,null,$Y);

        $blas->gemv(
            $order,$trans,
            $m,$n,
            $alpha,
            $AA,$offA,$ldA,
            $XX,$offX,$incX,
            $beta,
            $YY,$offY,$incY,
            $queue,$events
        );
        $events->wait();

        $this->assertEquals(
            [123,456]
        ,$Y->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([[1,2,3],[4,5,6]]),$dtype);
        $X = $this->array($this->toComplex([100,10,1]),$dtype);
        $Y = $this->ones([2],$dtype);

        [ $order,$trans,$m,$n,$alpha,$AA,$offA,$ldA,
          $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
            $this->translate_gemv($A,$X,null,null,$Y);

        $blas->gemv(
            $order,$trans,
            $m,$n,
            $alpha,
            $AA,$offA,$ldA,
            $XX,$offX,$incX,
            $beta,
            $YY,$offY,$incY,
            $queue,$events
        );
        $events->wait();

        $this->assertEquals(
            $this->toComplex([123,456]),
            $this->toComplex($Y->toArray())
        );

        // complex64 check imag
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [C(1,i:1),C(2,i:1),C(3,i:1)],
            [C(4),C(5),C(6)]
        ]),$dtype);
        $X = $this->array($this->toComplex([100,10,1]),$dtype);
        $Y = $this->ones([2],$dtype);
        $alpha = null;
        $beta  = null;

        [ $order,$trans,$m,$n,$alpha,$AA,$offA,$ldA,
          $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
            $this->translate_gemv($A,$X,$alpha,$beta,$Y);

        $blas->gemv(
            $order,$trans,
            $m,$n,
            $alpha,
            $AA,$offA,$ldA,
            $XX,$offX,$incX,
            $beta,
            $YY,$offY,$incY,
            $queue,$events);
        $events->wait();

        $this->assertEquals(
            $this->toComplex([C(123,i:111),C(456)]),
            $this->toComplex($Y->toArray())
        );

        // complex64 check imag has beta 1.0
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [C(1,i:1),C(2,i:1),C(3,i:1)],
            [C(4),C(5),C(6)]
        ]),$dtype);
        $X = $this->array($this->toComplex([100,10,1]),$dtype);
        $Y = $this->ones([2],$dtype);
        $alpha = null;
        $beta  = C(1.0);

        [ $order,$trans,$m,$n,$alpha,$AA,$offA,$ldA,
          $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
            $this->translate_gemv($A,$X,$alpha,$beta,$Y);

        $blas->gemv(
            $order,$trans,
            $m,$n,
            $alpha,
            $AA,$offA,$ldA,
            $XX,$offX,$incX,
            $beta,
            $YY,$offY,$incY,
            $queue,$events);
        $events->wait();

        $this->assertEquals(
            $this->toComplex([C(124,i:111),C(457)]),
            $this->toComplex($Y->toArray())
        );

        // complex64 check imag has beta 2.0
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [C(1,i:1),C(2,i:1),C(3,i:1)],
            [C(4),C(5),C(6)]
        ]),$dtype);
        $X = $this->array($this->toComplex([100,10,1]),$dtype);
        $Y = $this->ones([2],$dtype);
        $alpha = null;
        $beta  = C(2.0,i:1.0);

        [ $order,$trans,$m,$n,$alpha,$AA,$offA,$ldA,
          $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
            $this->translate_gemv($A,$X,$alpha,$beta,$Y);

        $blas->gemv(
            $order,$trans,
            $m,$n,
            $alpha,
            $AA,$offA,$ldA,
            $XX,$offX,$incX,
            $beta,
            $YY,$offY,$incY,
            $queue,$events);
        $events->wait();

        $this->assertEquals(
            $this->toComplex([C(125,i:112),C(458,i:1)]),
            $this->toComplex($Y->toArray())
        );

        // complex64 check imag has alpha 2.0
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [C(1,i:1),C(2,i:1),C(3,i:1)],
            [C(4),C(5),C(6)]
        ]),$dtype);
        $X = $this->array($this->toComplex([100,10,1]),$dtype);
        $Y = $this->ones([2],$dtype);
        $alpha = C(2.0,i:1.0);
        $beta  = null;

        [ $order,$trans,$m,$n,$alpha,$AA,$offA,$ldA,
          $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
            $this->translate_gemv($A,$X,$alpha,$beta,$Y);

        $blas->gemv(
            $order,$trans,
            $m,$n,
            $alpha,
            $AA,$offA,$ldA,
            $XX,$offX,$incX,
            $beta,
            $YY,$offY,$incY,
            $queue,$events);
        $events->wait();

        $this->assertEquals(
            $this->toComplex([C(135,i:345),C(912,i:456)]),
            $this->toComplex($Y->toArray())
        );
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $A = $this->array([[1,2,3],[4,5,6]],$dtype);
            $X = $this->array([100,10,1],$dtype);
            $Y = $this->ones([2],$dtype);

            [ $order,$trans,$m,$n,$alpha,$AA,$offA,$ldA,
              $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
                $this->translate_gemv($A,$X,null,null,$Y);

            $blas->gemv(
                $order,$trans,
                $m,$n,
                $alpha,
                $AA,$offA,$ldA,
                $XX,$offX,$incX,
                $beta,
                $YY,$offY,$incY,
                $queue,$events
            );
            $events->wait();

            $this->assertEquals(
                [123,456]
            ,$Y->toArray());

            /*
            // complex128
            $dtype = NDArray::complex128;
            $A = $this->array($this->toComplex([[1,2,3],[4,5,6]]),$dtype);
            $X = $this->array($this->toComplex([100,10,1]),$dtype);
            $Y = $this->ones([2],$dtype);

            [ $order,$trans,$m,$n,$alpha,$AA,$offA,$ldA,
              $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
                $this->translate_gemv($A,$X,null,null,$Y);

            $blas->gemv(
                $order,$trans,
                $m,$n,
                $alpha,
                $AA,$offA,$ldA,
                $XX,$offX,$incX,
                $beta,
                $YY,$offY,$incY,
                $queue,$events
            );
            $events->wait();

            $this->assertEquals(
                $this->toComplex([123,456]),
                $this->toComplex($Y->toArray())
            );
            */

        }

    }

    public function testGemvTranspose()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $A = $this->array([[1,2,3],[4,5,6]],$dtype);
        $X = $this->array([10,1],$dtype);
        $Y = $this->zeros([3],$dtype);

        [ $order,$trans,$m,$n,$alpha,$AA,$offA,$ldA,
          $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
            $this->translate_gemv($A,$X,null,null,$Y,true);

        $blas->gemv(
            $order,$trans,
            $m,$n,
            $alpha,
            $AA,$offA,$ldA,
            $XX,$offX,$incX,
            $beta,
            $YY,$offY,$incY,
            $queue,$events);
        $events->wait();

        $this->assertEquals(
            [14,25,36]
        ,$Y->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $A = $this->array([[C(1),C(2),C(3)],[C(4),C(5),C(6)]],$dtype);
        $X = $this->array([C(10),C(1)],$dtype);
        $Y = $this->zeros([3],$dtype);

        [ $order,$trans,$m,$n,$alpha,$AA,$offA,$n,
          $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
            $this->translate_gemv($A,$X,null,null,$Y,true);

        $blas->gemv(
            $order,$trans,
            $m,$n,
            $alpha,
            $AA,$offA,$n,
            $XX,$offX,$incX,
            $beta,
            $YY,$offY,$incY,
            $queue,$events);
        $events->wait();

        $this->assertEquals($this->toComplex(
            [14,25,36]
        ),$this->toComplex($Y->toArray()));

        // complex64 check trans and conj
        $dtype = NDArray::complex64;
        $A = $this->array([[C(1,i:1),C(2,i:1),C(3,i:1)],[C(4,i:1),C(5,i:1),C(6,i:1)]],$dtype);
        $X = $this->array([C(10),C(1)],$dtype);
        $Y = $this->zeros([3],$dtype);

        [ $order,$trans,$m,$n,$alpha,$AA,$offA,$n,
          $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
            $this->translate_gemv($A,$X,null,null,$Y,true);

        $blas->gemv(
            $order,$trans,
            $m,$n,
            $alpha,
            $AA,$offA,$n,
            $XX,$offX,$incX,
            $beta,
            $YY,$offY,$incY,
            $queue,$events);
        $events->wait();

        $this->assertEquals($this->toComplex(
            [C(14,i:-11),C(25,i:-11),C(36,i:-11)]
        ),$this->toComplex($Y->toArray()));

        // complex64 check trans and no_conj
        $dtype = NDArray::complex64;
        $A = $this->array([[C(1,i:1),C(2,i:1),C(3,i:1)],[C(4,i:1),C(5,i:1),C(6,i:1)]],$dtype);
        $X = $this->array([C(10),C(1)],$dtype);
        $Y = $this->zeros([3],$dtype);

        [ $order,$trans,$m,$n,$alpha,$AA,$offA,$n,
          $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
            $this->translate_gemv($A,$X,null,null,$Y,true,false); // trans:true, conj:false

        $blas->gemv(
            $order,$trans,
            $m,$n,
            $alpha,
            $AA,$offA,$n,
            $XX,$offX,$incX,
            $beta,
            $YY,$offY,$incY,
            $queue,$events);
        $events->wait();

        $this->assertEquals($this->toComplex(
            [C(14,i:11),C(25,i:11),C(36,i:11)]
        ),$this->toComplex($Y->toArray()));

        //  ** CAUTION **
        //  CLBlast does not support ConjNoTrans
        //
        // complex64 check no_trans and conj
        //$A = $this->array([[C(1,i:1),C(2,i:1),C(3,i:1)],[C(4,i:1),C(5,i:1),C(6,i:1)]],NDArray::complex64);
        //$X = $this->array([C(100),C(10),C(1)],NDArray::complex64);
        //$Y = $this->zeros([2],NDArray::complex64);
        //
        //[ $order,$trans,$m,$n,$alpha,$AA,$offA,$n,
        //  $XX,$offX,$incX,$beta,$YY,$offY,$incY,$queue,$events] =
        //    $this->translate_gemv($A,$X,null,null,$Y,false,true); // trans:false,conj:true
        //$blas->gemv(
        //    $order,$trans,
        //    $m,$n,
        //    $alpha,
        //    $AA,$offA,$n,
        //    $XX,$offX,$incX,
        //    $beta,
        //    $YY,$offY,$incY,
        //    $queue,$events);
        //$events->wait();
        //
        //$this->assertEquals($this->toComplex(
        //    [C(123,i:-111),C(456,i:-111)]
        //),$this->toComplex($Y->toArray()));
        */

    }

    protected function getGemvTestEnv($m,$n)
    {
        $ocl = $this->getOpenCL();
        $queue = $this->queue;
        $context = $queue->getContext();
        $hostBufferA = $this->newHostBuffer($m*$n,NDArray::float32);
        $hostBufferX = $this->newHostBuffer($n,NDArray::float32);
        $hostBufferY = $this->newHostBuffer($m,NDArray::float32);
        $testTruesR = $this->newHostBuffer($m,NDArray::float32);
        $alpha=2.0;
        $beta=0.5;
        for($i=0;$i<$m*$n;$i++) {
            $hostBufferA[$i]=random_int(0, 255)/256;
        }
        for($i=0;$i<$n;$i++) {
            $hostBufferX[$i]=random_int(0, 255)/256;
        }
        for($i=0;$i<$m;$i++) {
            $hostBufferY[$i]=random_int(0, 255)/256;
            $testTruesR[$i]=$hostBufferY[$i];
        }
        $openblas = $this->getOpenBLAS();
        $openblas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
            $alpha,
            $hostBufferA, 0, $n,
            $hostBufferX, 0, 1,
            $beta,
            $testTruesR,  0, 1
        );
        $bufferA = $ocl->Buffer($context,intval($m*$n*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferA);
        $bufferX = $ocl->Buffer($context,intval($n*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferY = $ocl->Buffer($context,intval($m*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferY);

        $blas = $this->getBlas();
        $events = $ocl->EventList();
        return [
            $queue,$blas,$events,$bufferA,$bufferX,$bufferY,
            $hostBufferA,$hostBufferX,$hostBufferY,$testTruesR,$alpha,$beta,
        ];
    }

    public function testGemvFullRange()
    {
        $m = 512;
        $n = 256;
        [
            $queue,$blas,$events,$bufferA,$bufferX,$bufferY,
            $hostBufferA,$hostBufferX,$hostBufferY,$testTruesR,$alpha,$beta,
        ] = $this->getGemvTestEnv($m,$n);
        $blas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$n,
            $bufferX,$offsetX=0,$incX=1,
            $beta,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
        $events->wait();
        $bufferY->read($queue,$hostBufferY);
        $equals = true;
        for($i=0;$i<$m;$i++) {
            if($hostBufferY[$i]!=$testTruesR[$i]) {
                $equals = false;
                break;
            }
        }
        $this->assertTrue($equals);
    }

    public function testGemvInvalidBufferAObject()
    {
        $m = 512;
        $n = 256;
        [
            $queue,$blas,$events,$bufferA,$bufferX,$bufferY,
            $hostBufferA,$hostBufferX,$hostBufferY,$testTruesR,$alpha,$beta,
        ] = $this->getGemvTestEnv($m,$n);
        // invalid buffer object arguments
        $bufferA = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$n,
            $bufferX,$offsetX=0,$incX=1,
            $beta,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
    }

    public function testGemvInvalidBufferXObject()
    {
        $m = 512;
        $n = 256;
        [
            $queue,$blas,$events,$bufferA,$bufferX,$bufferY,
            $hostBufferA,$hostBufferX,$hostBufferY,$testTruesR,$alpha,$beta,
        ] = $this->getGemvTestEnv($m,$n);
        // invalid buffer object arguments
        $bufferX = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$n,
            $bufferX,$offsetX=0,$incX=1,
            $beta,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
    }

    public function testGemvInvalidBufferYObject()
    {
        $m = 512;
        $n = 256;
        [
            $queue,$blas,$events,$bufferA,$bufferX,$bufferY,
            $hostBufferA,$hostBufferX,$hostBufferY,$testTruesR,$alpha,$beta,
        ] = $this->getGemvTestEnv($m,$n);
        // invalid buffer object arguments
        $bufferY = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$n,
            $bufferX,$offsetX=0,$incX=1,
            $beta,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
    }

    public function testGemvInvalidQueueObject()
    {
        $m = 512;
        $n = 256;
        [
            $queue,$blas,$events,$bufferA,$bufferX,$bufferY,
            $hostBufferA,$hostBufferX,$hostBufferY,$testTruesR,$alpha,$beta,
        ] = $this->getGemvTestEnv($m,$n);
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$n,
            $bufferX,$offsetX=0,$incX=1,
            $beta,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
    }

    public function testGemvInvalidEventsObject()
    {
        $m = 512;
        $n = 256;
        [
            $queue,$blas,$events,$bufferA,$bufferX,$bufferY,
            $hostBufferA,$hostBufferX,$hostBufferY,$testTruesR,$alpha,$beta,
        ] = $this->getGemvTestEnv($m,$n);
        // invalid Events object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->gemv(BLAS::RowMajor,BLAS::NoTrans,$m,$n,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$n,
            $bufferX,$offsetX=0,$incX=1,
            $beta,
            $bufferY,$offsetY=0,$incY=1,
            $queue,$events);
    }

    //
    //  gemm
    //

    protected function translate_gemm(
        NDArray $A,
        NDArray $B,
        $alpha=null,
        $beta=null,
        NDArray $C=null,
        bool $transA=null,
        bool $transB=null,
        bool $conjA=null,
        bool $conjB=null,
        object $events=null
        )
    {
        [$transA,$conjA] = $this->complementTrans($transA,$conjA,$A->dtype());
        [$transB,$conjB] = $this->complementTrans($transB,$conjB,$B->dtype());

        $shapeA = $A->shape();
        if($transA) {
            $shapeA = [$shapeA[1],$shapeA[0]];
        }
        $shapeB = $B->shape();
        if($transB) {
            $shapeB = [$shapeB[1],$shapeB[0]];
        }
        if($shapeA[1]!=$shapeB[0]) {
            throw new InvalidArgumentException('The number of columns in "A" and the number of rows in "B" must be the same');
        }
        $AA = $A->buffer();
        $BB = $B->buffer();
        $offA = $A->offset();
        $offB = $B->offset();
        $M = $shapeA[0];
        $N = $shapeB[1];
        $K = $shapeA[1];

        if($alpha===null) {
            //if($this->isComplex($A->dtype())) {
            //    $alpha = C(1.0);
            //} else {
                $alpha = 1.0;
            //}
        }
        if($beta===null) {
            //if($this->isComplex($A->dtype())) {
            //    $beta = C(0.0);
            //} else {
                $beta = 0.0;
            //}
        }
        if($C!=null) {
            $shapeC = $C->shape();
            if($M!=$shapeC[0] || $N!=$shapeC[1]) {
                throw new InvalidArgumentException('"A" and "C" must have the same number of rows."B" and "C" must have the same number of columns');
            }
        } else {
            $C = $this->zeros($this->alloc([$M,$N],$A->dtype()));
        }
        $CC = $C->buffer();
        $offC = $C->offset();

        $lda = ($transA) ? $M : $K;
        $ldb = ($transB) ? $K : $N;
        $ldc = $N;
        $transA = $this->transToCode($transA,$conjA);
        $transB = $this->transToCode($transB,$conjB);
        $order = BLAS::RowMajor;
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }

        return [
            $order,$transA,$transB,
            $M,$N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $this->queue,$events,
        ];
    }

    public function testGemmNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $A = $this->array([[1,2,3],[4,5,6],[7,8,9]],$dtype);
        $B = $this->array([[1,0,0],[0,1,0],[0,0,1]],$dtype);
        $alpha = 1.0;
        $beta  = 0.0;
        $C = $this->ones([3,3],$dtype);
        $transA = false;
        $transB = false;

        [ $order,$transA,$transB,$M,$N,$K,$alpha,$AA,$offA,$lda,
          $BB,$offB,$ldb,$beta,$CC,$offC,$ldc,$queue,$events] =
            $this->translate_gemm($A,$B,$alpha,$beta,$C,$transA,$transB);

        $blas->gemm(
            $order,$transA,$transB,
            $M,$N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events
        );
        $events->wait();

        $this->assertEquals([
            [1,2,3],
            [4,5,6],
            [7,8,9]
        ],$C->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([[1,2,3],[4,5,6],[7,8,9]]),$dtype);
        $B = $this->array($this->toComplex([[1,0,0],[0,1,0],[0,0,1]]),$dtype);
        $alpha = C(1.0);
        $beta  = C(0.0);
        $C = $this->ones([3,3],$dtype);
        $transA = false;
        $transB = false;

        [ $order,$transA,$transB,$M,$N,$K,$alpha,$AA,$offA,$lda,
          $BB,$offB,$ldb,$beta,$CC,$offC,$ldc,$queue,$events] =
            $this->translate_gemm($A,$B,$alpha,$beta,$C,$transA,$transB);

        $blas->gemm(
            $order,$transA,$transB,
            $M,$N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        );
        $events->wait();

        $this->assertEquals($this->toComplex([
            [1,2,3],
            [4,5,6],
            [7,8,9]
        ]),$this->toComplex($C->toArray()));

        // complex64 check imag
        $dtype = NDArray::complex64;
        $A = $this->array([[C(1,i:1),C(2,i:1),C(3,i:1)],[C(4),C(5),C(6)],[C(7),C(8),C(9)]],$dtype);
        $B = $this->array([[C(1,i:1),C(0,i:1),C(0,i:1)],[C(0),C(1),C(0)],[C(0),C(0),C(1)]],$dtype);
        $alpha = null;
        $beta  = null;
        $C = $this->ones([3,3],$dtype);
        $transA = false;
        $transB = false;

        [ $order,$transA,$transB,$M,$N,$K,$alpha,$AA,$offA,$lda,
          $BB,$offB,$ldb,$beta,$CC,$offC,$ldc,$queue,$events] =
            $this->translate_gemm($A,$B,$alpha,$beta,$C,$transA,$transB);

        $blas->gemm(
            $order,$transA,$transB,
            $M,$N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events);
        $events->wait();

        $this->assertEquals($this->toComplex([
            [C(0,i:2),C(1,i:2),C(2,i:2)],
            [C(4,i:4),C(5,i:4),C(6,i:4)],
            [C(7,i:7),C(8,i:7),C(9,i:7)]
        ]),$this->toComplex($C->toArray()));

        // complex64 check imag has beta 1.0
        $dtype = NDArray::complex64;
        $A = $this->array([[C(1,i:1),C(2,i:1),C(3,i:1)],[C(4),C(5),C(6)],[C(7),C(8),C(9)]],$dtype);
        $B = $this->array([[C(1,i:1),C(0,i:1),C(0,i:1)],[C(0),C(1),C(0)],[C(0),C(0),C(1)]],$dtype);
        $alpha = null;
        $beta  = C(1.0);
        $C = $this->ones([3,3],$dtype);
        $transA = false;
        $transB = false;

        [ $order,$transA,$transB,$M,$N,$K,$alpha,$AA,$offA,$lda,
          $BB,$offB,$ldb,$beta,$CC,$offC,$ldc,$queue,$events] =
            $this->translate_gemm($A,$B,$alpha,$beta,$C,$transA,$transB);

        $blas->gemm(
            $order,$transA,$transB,
            $M,$N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events
        );
        $events->wait();

        $this->assertEquals($this->toComplex([
            [C(1,i:2),C(2,i:2),C(3,i:2)],
            [C(5,i:4),C(6,i:4),C(7,i:4)],
            [C(8,i:7),C(9,i:7),C(10,i:7)]
        ]),$this->toComplex($C->toArray()));

        // complex64 check imag has beta 2.0
        $dtype = NDArray::complex64;
        $A = $this->array([[C(1,i:1),C(2,i:1),C(3,i:1)],[C(4),C(5),C(6)],[C(7),C(8),C(9)]],$dtype);
        $B = $this->array([[C(1,i:1),C(0,i:1),C(0,i:1)],[C(0),C(1),C(0)],[C(0),C(0),C(1)]],$dtype);
        $alpha = null;
        $beta  = C(2.0,i:1.0);
        $C = $this->ones([3,3],$dtype);
        $transA = false;
        $transB = false;

        [ $order,$transA,$transB,$M,$N,$K,$alpha,$AA,$offA,$lda,
          $BB,$offB,$ldb,$beta,$CC,$offC,$ldc,$queue,$events] =
            $this->translate_gemm($A,$B,$alpha,$beta,$C,$transA,$transB);

        $blas->gemm(
            $order,$transA,$transB,
            $M,$N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events
        );
        $events->wait();

        $this->assertEquals($this->toComplex([
            [C(2,i:3),C(3,i:3),C(4,i:3)],
            [C(6,i:5),C(7,i:5),C(8,i:5)],
            [C(9,i:8),C(10,i:8),C(11,i:8)]
        ]),$this->toComplex($C->toArray()));

        // complex64 check imag has alpha 2.0
        $dtype = NDArray::complex64;
        $A = $this->array([[C(1,i:1),C(2,i:1),C(3,i:1)],[C(4),C(5),C(6)],[C(7),C(8),C(9)]],$dtype);
        $B = $this->array([[C(1,i:1),C(0,i:1),C(0,i:1)],[C(0),C(1),C(0)],[C(0),C(0),C(1)]],$dtype);
        $alpha = C(2.0,i:1.0);
        $beta  = null;
        $C = $this->ones([3,3],$dtype);
        $transA = false;
        $transB = false;

        [ $order,$transA,$transB,$M,$N,$K,$alpha,$AA,$offA,$lda,
          $BB,$offB,$ldb,$beta,$CC,$offC,$ldc,$queue,$events] =
            $this->translate_gemm($A,$B,$alpha,$beta,$C,$transA,$transB);

        $blas->gemm(
            $order,$transA,$transB,
            $M,$N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events
        );
        $events->wait();

        $this->assertEquals($this->toComplex([
            [C(-2,i:4),C(0,i:5),C(2,i:6)],
            [C(4,i:12),C(6,i:13),C(8,i:14)],
            [C(7,i:21),C(9,i:22),C(11,i:23)]
        ]),$this->toComplex($C->toArray()));
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $A = $this->array([[1,2,3],[4,5,6],[7,8,9]],$dtype);
            $B = $this->array([[1,0,0],[0,1,0],[0,0,1]],$dtype);
            $alpha = 1.0;
            $beta  = 0.0;
            $C = $this->ones([3,3],$dtype);
            $transA = false;
            $transB = false;

            [ $order,$transA,$transB,$M,$N,$K,$alpha,$AA,$offA,$lda,
              $BB,$offB,$ldb,$beta,$CC,$offC,$ldc,$queue,$events] =
                $this->translate_gemm($A,$B,$alpha,$beta,$C,$transA,$transB);

            $blas->gemm(
                $order,$transA,$transB,
                $M,$N,$K,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events
            );
            $events->wait();

            $this->assertEquals([
                [1,2,3],
                [4,5,6],
                [7,8,9]
            ],$C->toArray());

            /*
            // complex128
            $dtype = NDArray::complex128;
            $A = $this->array($this->toComplex([[1,2,3],[4,5,6],[7,8,9]]),$dtype);
            $B = $this->array($this->toComplex([[1,0,0],[0,1,0],[0,0,1]]),$dtype);
            $alpha = C(1.0);
            $beta  = C(0.0);
            $C = $this->ones([3,3],$dtype);
            $transA = false;
            $transB = false;

            [ $order,$transA,$transB,$M,$N,$K,$alpha,$AA,$offA,$lda,
              $BB,$offB,$ldb,$beta,$CC,$offC,$ldc,$queue,$events] =
                $this->translate_gemm($A,$B,$alpha,$beta,$C,$transA,$transB);

            $blas->gemm(
                $order,$transA,$transB,
                $M,$N,$K,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events,
            );
            $events->wait();

            $this->assertEquals($this->toComplex([
                [1,2,3],
                [4,5,6],
                [7,8,9]
            ]),$this->toComplex($C->toArray()));
            */

        }
    }

    protected function getGemmTestEnv($m,$n,$k)
    {
        $ocl = $this->getOpenCL();
        $queue = $this->queue;
        $context = $queue->getContext();
        $hostBufferA = $this->newHostBuffer($m*$k,NDArray::float32);
        $hostBufferB = $this->newHostBuffer($k*$n,NDArray::float32);
        $hostBufferC = $this->newHostBuffer($m*$n,NDArray::float32);
        $testTruesR  = $this->newHostBuffer($m*$n,NDArray::float32);
        $alpha=2.0;
        $beta=0.5;
        for($i=0;$i<$m*$k;$i++) {
            $hostBufferA[$i]=random_int(0, 255)/256;
        }
        for($i=0;$i<$k*$n;$i++) {
            $hostBufferB[$i]=random_int(0, 255)/256;
        }
        for($i=0;$i<$m*$n;$i++) {
            $hostBufferC[$i]=random_int(0, 255)/256;
            $testTruesR[$i]=$hostBufferC[$i];
        }
        $openblas = $this->getOpenBLAS();
        $openblas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
            $alpha,
            $hostBufferA, 0, $k,
            $hostBufferB, 0, $n,
            $beta,
            $testTruesR,  0, $n
        );
        $bufferA = $ocl->Buffer($context,intval($m*$k*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferA);
        $bufferB = $ocl->Buffer($context,intval($k*$n*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferB);
        $bufferC = $ocl->Buffer($context,intval($m*$n*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferC);

        $blas = $this->getBlas();
        $events = $ocl->EventList();
        return [
            $queue,$blas,$events,$bufferA,$bufferB,$bufferC,
            $hostBufferA,$hostBufferB,$hostBufferC,$testTruesR,$alpha,$beta,
        ];
    }

    public function testGemmFullRange()
    {
        $m = 512;
        $n = 256;
        $k = 256;
        [
            $queue,$blas,$events,$bufferA,$bufferB,$bufferC,
            $hostBufferA,$hostBufferB,$hostBufferC,$testTruesR,$alpha,$beta,
        ] = $this->getGemmTestEnv($m,$n,$k);
        $blas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$k,
            $bufferB,$offsetB=0,$ldA=$n,
            $beta,
            $bufferC,$offsetC=0,$ldC=$n,
            $queue,$events);
        $events->wait();
        $bufferC->read($queue,$hostBufferC);
        $equals = true;
        for($i=0;$i<$m*$n;$i++) {
            if($hostBufferC[$i]!=$testTruesR[$i]) {
                $equals = false;
                break;
            }
        }
        $this->assertTrue($equals);
    }

    public function testGemmInvalidBufferAObject()
    {
        $m = 512;
        $n = 256;
        $k = 256;
        [
            $queue,$blas,$events,$bufferA,$bufferB,$bufferC,
            $hostBufferA,$hostBufferB,$hostBufferC,$testTruesR,$alpha,$beta,
        ] = $this->getGemmTestEnv($m,$n,$k);
        // invalid buffer object arguments
        $bufferA = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$k,
            $bufferB,$offsetB=0,$ldA=$n,
            $beta,
            $bufferC,$offsetC=0,$ldC=$n,
            $queue,$events);
    }

    public function testGemmInvalidBufferBObject()
    {
        $m = 512;
        $n = 256;
        $k = 256;
        [
            $queue,$blas,$events,$bufferA,$bufferB,$bufferC,
            $hostBufferA,$hostBufferB,$hostBufferC,$testTruesR,$alpha,$beta,
        ] = $this->getGemmTestEnv($m,$n,$k);
        // invalid buffer object arguments
        $bufferB = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$k,
            $bufferB,$offsetB=0,$ldA=$n,
            $beta,
            $bufferC,$offsetC=0,$ldC=$n,
            $queue,$events);
    }

    public function testGemmInvalidBufferCObject()
    {
        $m = 512;
        $n = 256;
        $k = 256;
        [
            $queue,$blas,$events,$bufferA,$bufferB,$bufferC,
            $hostBufferA,$hostBufferB,$hostBufferC,$testTruesR,$alpha,$beta,
        ] = $this->getGemmTestEnv($m,$n,$k);
        // invalid buffer object arguments
        $bufferC = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$k,
            $bufferB,$offsetB=0,$ldA=$n,
            $beta,
            $bufferC,$offsetC=0,$ldC=$n,
            $queue,$events);
    }

    public function testGemmInvalidQueueObject()
    {
        $m = 512;
        $n = 256;
        $k = 256;
        [
            $queue,$blas,$events,$bufferA,$bufferB,$bufferC,
            $hostBufferA,$hostBufferB,$hostBufferC,$testTruesR,$alpha,$beta,
        ] = $this->getGemmTestEnv($m,$n,$k);
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$k,
            $bufferB,$offsetB=0,$ldA=$n,
            $beta,
            $bufferC,$offsetC=0,$ldC=$n,
            $queue,$events);
    }

    public function testGemmInvalidEventsObject()
    {
        $m = 512;
        $n = 256;
        $k = 256;
        [
            $queue,$blas,$events,$bufferA,$bufferB,$bufferC,
            $hostBufferA,$hostBufferB,$hostBufferC,$testTruesR,$alpha,$beta,
        ] = $this->getGemmTestEnv($m,$n,$k);
        // invalid Events object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $blas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$k,
            $bufferB,$offsetB=0,$ldA=$n,
            $beta,
            $bufferC,$offsetC=0,$ldC=$n,
            $queue,$events);
    }

    public function testGemmNoWait()
    {
        if($this->skipDisplayInfo) {
            $this->markTestSkipped('Skip Display time to calculate.');
            return;
        }
        $m = 512;
        $n = 512;
        $k = 512;
        [
            $queue,$blas,$events,$bufferA,$bufferB,$bufferC,
            $hostBufferA,$hostBufferB,$hostBufferC,$testTruesR,$alpha,$beta,
        ] = $this->getGemmTestEnv($m,$n,$k);

        // preloading
        $blas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$k,
            $bufferB,$offsetB=0,$ldA=$n,
            $beta,
            $bufferC,$offsetC=0,$ldC=$n,
            $queue,$events);
        $events->wait();
        //
        // wait
        //
        #$start = hrtime(true);
        $start = microtime(true);
        for($i=0;$i<5;$i++) {
            $blas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
                $alpha,
                $bufferA,$offsetA=0,$ldA=$k,
                $bufferB,$offsetB=0,$ldA=$n,
                $beta,
                $bufferC,$offsetC=0,$ldC=$n,
                $queue,$events);
            $events->wait();
        }
        $enq = microtime(true);
        $queue->finish();
        $end = microtime(true);
        echo "\n";
        echo "==wait==\n";
        echo "total time=".($end-$start)."\n";
        echo "enqueue time=".($enq-$start)."\n";
        echo "wait time=".($end-$enq)."\n";
        //
        // no wait
        //
        #$end = hrtime(true);
        #$start = hrtime(true);
        $start = microtime(true);
        for($i=0;$i<5;$i++) {
            $blas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
                $alpha,
                $bufferA,$offsetA=0,$ldA=$k,
                $bufferB,$offsetB=0,$ldA=$n,
                $beta,
                $bufferC,$offsetC=0,$ldC=$n,
                $queue);//,$events);
            //$events->wait();
        }
        $enq = microtime(true);
        $queue->finish();
        $end = microtime(true);
        echo "\n";
        echo "==no wait==\n";
        echo "total time=".($end-$start)."\n";
        echo "enqueue time=".($enq-$start)."\n";
        echo "wait time=".($end-$enq)."\n";
        #$end = hrtime(true);
        $this->assertTrue(true);
    }

    //
    //  symm
    //

    public function translate_symm(
        NDArray $A,
        NDArray $B,
        $alpha=null,
        $beta=null,
        NDArray $C=null,
        bool $right=null,
        bool $lower=null,
        object $events=null
        ) : array
    {
        if($A->ndim()!=2 || $B->ndim()!=2) {
            throw new InvalidArgumentException('Dimensions must be 2D-NDArray');
        }
        $shapeA = $A->shape();
        $rowsA = $shapeA[0];
        if($rowsA!=$shapeA[1]) {
            throw new InvalidArgumentException('The matrix "A" must be symmetric');
        }
        $shapeB = $B->shape();
        $M = $shapeB[0];
        $N = $shapeB[1];
        $tmpB = ($right) ? $N : $M;
        if($rowsA!=$tmpB) {
            throw new InvalidArgumentException('Unmatch Shape of matrix "A" and "B": '."($rowsA,$rowsA) != ($M,$N)");
        }
        $AA = $A->buffer();
        $BB = $B->buffer();
        $offA = $A->offset();
        $offB = $B->offset();

        if($alpha===null) {
            //if($this->isComplex($A->dtype())) {
            //    $alpha = C(1.0);
            //} else {
                $alpha = 1.0;
            //}
        }
        if($beta===null) {
            //if($this->isComplex($A->dtype())) {
            //    $beta = C(0.0);
            //} else {
                $beta = 0.0;
            //}
        }
        if($C!=null) {
            $shapeC = $C->shape();
            if($M!=$shapeC[0] || $N!=$shapeC[1]) {
                throw new InvalidArgumentException('Matrix "B" and "C" must be same shape');
            }
        } else {
            $C = $this->zeros($this->alloc([$M,$N],$A->dtype()));
        }
        $CC = $C->buffer();
        $offC = $C->offset();

        $lda = $rowsA;
        $ldb = $N;
        $ldc = $N;
        $side = ($right) ? BLAS::Right : BLAS::Left;
        $uplo = ($lower) ? BLAS::Lower : BLAS::Upper;
        $order = BLAS::RowMajor;
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }

        return [
            $order,$side,$uplo,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $this->queue,$events,
        ];
    }

    public function testSymmNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $A = $this->array([
            [1,2,3],
            [2,4,5],
            [3,5,6],
        ],$dtype);
        $B = $this->array([
            [1,2,3,4],
            [5,6,7,8],
            [9,10,11,12],
        ],$dtype);
        $alpha = 1.0;
        $beta  = 0.0;
        $C = $this->zeros([3,4],$dtype);

        [
            $order,$side,$uplo,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        ] = $this->translate_symm($A,$B,$alpha,$beta,$C);

        $blas->symm(
            $order,
            $side,$uplo,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events
        );

        $this->assertEquals([
            [38, 44, 50, 56],
            [67, 78, 89,100],
            [82, 96,110,124]
        ],$C->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [1,2,3],
            [2,4,5],
            [3,5,6],
        ]),$dtype);
        $B = $this->array($this->toComplex([
            [1,2,3,4],
            [5,6,7,8],
            [9,10,11,12],
        ]),$dtype);
        $alpha = C(1.0);
        $beta  = C(0.0);
        $C = $this->zeros([3,4],$dtype);

        [
            $order,$side,$uplo,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        ] = $this->translate_symm($A,$B,$alpha,$beta,$C);

        $blas->symm(
            $order,
            $side,$uplo,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        );

        $this->assertEquals($this->toComplex([
            [38, 44, 50, 56],
            [67, 78, 89,100],
            [82, 96,110,124]
        ]),$this->toComplex($C->toArray()));
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $A = $this->array([
                [1,2,3],
                [2,4,5],
                [3,5,6],
            ],$dtype);
            $B = $this->array([
                [1,2,3,4],
                [5,6,7,8],
                [9,10,11,12],
            ],$dtype);
            $alpha = 1.0;
            $beta  = 0.0;
            $C = $this->zeros([3,4],$dtype);

            [
                $order,$side,$uplo,
                $M,$N,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events,
            ] = $this->translate_symm($A,$B,$alpha,$beta,$C);

            $blas->symm(
                $order,
                $side,$uplo,
                $M,$N,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events
            );

            $this->assertEquals([
                [38, 44, 50, 56],
                [67, 78, 89,100],
                [82, 96,110,124]
            ],$C->toArray());

            /*
            // complex128
            $dtype = NDArray::complex128;
            $A = $this->array($this->toComplex([
                [1,2,3],
                [2,4,5],
                [3,5,6],
            ]),$dtype);
            $B = $this->array($this->toComplex([
                [1,2,3,4],
                [5,6,7,8],
                [9,10,11,12],
            ]),$dtype);
            $alpha = C(1.0);
            $beta  = C(0.0);
            $C = $this->zeros([3,4],$dtype);
    
            [
                $order,$side,$uplo,
                $M,$N,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events,
            ] = $this->translate_symm($A,$B,$alpha,$beta,$C);
    
            $blas->symm(
                $order,
                $side,$uplo,
                $M,$N,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events,
            );
    
            $this->assertEquals($this->toComplex([
                [38, 44, 50, 56],
                [67, 78, 89,100],
                [82, 96,110,124]
            ]),$this->toComplex($C->toArray()));
            */
    
        }
    }

    //
    //  syrk
    //

    public function translate_syrk(
        NDArray $A,
        $alpha=null,
        $beta=null,
        NDArray $C=null,
        bool $lower=null,
        bool $trans=null,
        bool $conj=null,
        object $events=null
        ) : array
    {
        $trans = $trans ?? false;
        // $conj = $conj ?? $trans; // Doing so will result in an error.
        $conj = false;  // conj must be false

        if($A->ndim()!=2) {
            throw new InvalidArgumentException('Dimensions must be 2D-NDArray');
        }

        $shapeA = $A->shape();
        if($trans) {
            $shapeA = [$shapeA[1],$shapeA[0]];
        }
        $AA = $A->buffer();
        $offA = $A->offset();
        $N = $shapeA[0];
        $K = $shapeA[1];

        if($alpha===null) {
            //if($this->isComplex($A->dtype())) {
            //    $alpha = C(1.0);
            //} else {
                $alpha = 1.0;
            //}
        }
        if($beta===null) {
            //if($this->isComplex($A->dtype())) {
            //    $beta = C(0.0);
            //} else {
                $beta = 0.0;
            //}
        }
        if($C!=null) {
            $shapeC = $C->shape();
            if($N!=$shapeC[0] || $N!=$shapeC[1]) {
                throw new InvalidArgumentException('"C" rows and cols must have the same number of "A" cols');
            }
        } else {
            $C = $this->zeros($this->alloc([$N,$N],$A->dtype()));
        }
        $CC = $C->buffer();
        $offC = $C->offset();

        $lda = ($trans) ? $N : $K;
        $ldc = $N;
        $uplo  = ($lower) ? BLAS::Lower : BLAS::Upper;
        $trans = $this->transToCode($trans,$conj);
        $order = BLAS::RowMajor;
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }

        return [
            $order,$uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $beta,
            $CC,$offC,$ldc,
            $this->queue,$events,
        ];
    }

    public function testSyrkNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $A = $this->array([
            [ 1, 2, 3],
            [ 4, 5, 6],
            [ 7, 8, 9],
            [10,11,12],
        ],$dtype);
        $C = $this->zeros([4,4],$dtype);

        [
            $order,$uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        ] = $this->translate_syrk($A,null,null,$C);

        $blas->syrk(
            $order,
            $uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events
        );

        $this->assertEquals([
            [14, 32, 50, 68],
            [ 0, 77,122,167],
            [ 0,  0,194,266],
            [ 0,  0,  0,365],
        ],$C->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [ 1, 2, 3],
            [ 4, 5, 6],
            [ 7, 8, 9],
            [10,11,12],
        ]),$dtype);
        $C = $this->zeros([4,4],$dtype);

        [
            $order,$uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        ] = $this->translate_syrk($A,null,null,$C);

        $blas->syrk(
            $order,
            $uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        );

        $this->assertEquals($this->toComplex([
            [14, 32, 50, 68],
            [ 0, 77,122,167],
            [ 0,  0,194,266],
            [ 0,  0,  0,365],
        ]),$this->toComplex($C->toArray()));

        // complex64 check imag
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [C( 1,i:1),C( 2,i:1),C( 3,i:1)],
            [C( 4,i:1),C( 5,i:1),C( 6,i:1)],
            [C( 7,i:1),C( 8,i:1),C( 9,i:1)],
            [C(10,i:1),C(11,i:1),C(12,i:1)],
        ]),$dtype);
        $C = $this->zeros([4,4],$dtype);

        [
            $order,$uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        ] = $this->translate_syrk($A,null,null,$C);

        $blas->syrk(
            $order,
            $uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        );

        $this->assertEquals($this->toComplex([
            [C(11,i:12),C(29,i:21),C( 47,i:30),C( 65,i:39)],
            [C( 0,i: 0),C(74,i:30),C(119,i:39),C(164,i:48)],
            [C( 0,i: 0),C( 0,i: 0),C(191,i:48),C(263,i:57)],
            [C( 0,i: 0),C( 0,i: 0),C(  0,i: 0),C(362,i:66)],
        ]),$this->toComplex($C->toArray()));
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $A = $this->array([
                [ 1, 2, 3],
                [ 4, 5, 6],
                [ 7, 8, 9],
                [10,11,12],
            ],$dtype);
            $C = $this->zeros([4,4],$dtype);

            [
                $order,$uplo,$trans,
                $N,$K,
                $alpha,
                $AA,$offA,$lda,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events,
            ] = $this->translate_syrk($A,null,null,$C);

            $blas->syrk(
                $order,
                $uplo,$trans,
                $N,$K,
                $alpha,
                $AA,$offA,$lda,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events
            );

            $this->assertEquals([
                [14, 32, 50, 68],
                [ 0, 77,122,167],
                [ 0,  0,194,266],
                [ 0,  0,  0,365],
            ],$C->toArray());

            /*
            // complex128
            $dtype = NDArray::complex128;
            $A = $this->array($this->toComplex([
                [ 1, 2, 3],
                [ 4, 5, 6],
                [ 7, 8, 9],
                [10,11,12],
            ]),$dtype);
            $C = $this->zeros([4,4],$dtype);

            [
                $order,$uplo,$trans,
                $N,$K,
                $alpha,
                $AA,$offA,$lda,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events,
            ] = $this->translate_syrk($A,null,null,$C);

            $blas->syrk(
                $order,
                $uplo,$trans,
                $N,$K,
                $alpha,
                $AA,$offA,$lda,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events,
            );

            $this->assertEquals($this->toComplex([
                [14, 32, 50, 68],
                [ 0, 77,122,167],
                [ 0,  0,194,266],
                [ 0,  0,  0,365],
            ]),$this->toComplex($C->toArray()));
            */

        }
    }

    //
    // syr2k
    //

    public function translate_syr2k(
        NDArray $A,
        NDArray $B,
        $alpha=null,
        $beta=null,
        NDArray $C=null,
        bool $lower=null,
        bool $trans=null,
        bool $conj=null,
        object $events=null
        ) : array
    {
        $trans = $trans ?? false;
        // $conj = $conj ?? $trans; // Doing so will result in an error.
        $conj = false;  // conj must be false

        if($A->ndim()!=2 || $B->ndim()!=2) {
            throw new InvalidArgumentException('Dimensions must be 2D-NDArray');
        }
        $shapeA = $A->shape();
        $shapeB = $B->shape();
        if($shapeA!=$shapeB) {
            throw new InvalidArgumentException('Matrix A and B must be same shape');
        }
        if($trans) {
            $shapeA = [$shapeA[1],$shapeA[0]];
        }
        $AA   = $A->buffer();
        $offA = $A->offset();
        $BB   = $B->buffer();
        $offB = $B->offset();
        $N = $shapeA[0];
        $K = $shapeA[1];

        if($alpha===null) {
            //if($this->isComplex($A->dtype())) {
            //    $alpha = C(1.0);
            //} else {
                $alpha = 1.0;
            //}
        }
        if($beta===null) {
            //if($this->isComplex($A->dtype())) {
            //    $beta = C(0.0);
            //} else {
                $beta = 0.0;
            //}
        }
        if($C!=null) {
            $shapeC = $C->shape();
            if($N!=$shapeC[0] || $N!=$shapeC[1]) {
                throw new InvalidArgumentException('"C" rows and cols must have the same number of "A" cols');
            }
        } else {
            $C = $this->zeros($this->alloc([$N,$N],$A->dtype()));
        }
        $CC = $C->buffer();
        $offC = $C->offset();

        $lda = ($trans) ? $N : $K;
        $ldb = ($trans) ? $N : $K;
        $ldc = $N;
        $uplo  = ($lower) ? BLAS::Lower : BLAS::Upper;
        $trans = $this->transToCode($trans,$conj);
        $order = BLAS::RowMajor;
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }

        return [
            $order,$uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $this->queue,$events,
        ];
    }

    public function testSyr2kNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $A = $this->array([
            [1,2,3],
            [4,5,6],
            [7,8,9],
            [10,11,12],
        ],$dtype);
        $B = $this->array([
            [1,3,5],
            [2,4,6],
            [7,9,11],
            [8,10,12],
        ],$dtype);
        $C = $this->zeros([4,4],$dtype);

        [
            $order,$uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        ] = $this->translate_syr2k($A,$B,null,null,$C);

        $blas->syr2k(
            $order,
            $uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events
        );

        $this->assertEquals([
            [44, 77,134,167],
            [ 0,128,239,290],
            [ 0,  0,440,545],
            [ 0,  0,  0,668]
        ],$C->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [1,2,3],
            [4,5,6],
            [7,8,9],
            [10,11,12],
        ]),$dtype);
        $B = $this->array($this->toComplex([
            [1,3,5],
            [2,4,6],
            [7,9,11],
            [8,10,12],
        ]),$dtype);
        $C = $this->zeros([4,4],$dtype);

        [
            $order,$uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        ] = $this->translate_syr2k($A,$B,null,null,$C);

        $blas->syr2k(
            $order,
            $uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        );

        $this->assertEquals($this->toComplex([
            [44, 77,134,167],
            [ 0,128,239,290],
            [ 0,  0,440,545],
            [ 0,  0,  0,668]
        ]),$this->toComplex($C->toArray()));

        // complex64 check imag
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [C(1,i:1),C(2,i:1),C(3,i:1)],
            [4,5,6],
            [7,8,9],
            [10,11,12],
        ]),$dtype);
        $B = $this->array($this->toComplex([
            [1,3,5],
            [2,4,6],
            [7,9,11],
            [8,10,12],
        ]),$dtype);
        $C = $this->zeros([4,4],$dtype);

        [
            $order,$uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        ] = $this->translate_syr2k($A,$B,null,null,$C);

        $blas->syr2k(
            $order,
            $uplo,$trans,
            $N,$K,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $beta,
            $CC,$offC,$ldc,
            $queue,$events,
        );

        $this->assertEquals($this->toComplex([
            [C(44,i:18),C( 77,i:12),C(134,i:27),C(167,i:30)],
            [C( 0,i: 0),C(128,i: 0),C(239,i: 0),C(290,i: 0)],
            [C( 0,i: 0),C(  0,i: 0),C(440,i: 0),C(545,i: 0)],
            [C( 0,i: 0),C(  0,i: 0),C(  0,i: 0),C(668,i: 0)],
        ]),$this->toComplex($C->toArray()));
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $A = $this->array([
                [1,2,3],
                [4,5,6],
                [7,8,9],
                [10,11,12],
            ],$dtype);
            $B = $this->array([
                [1,3,5],
                [2,4,6],
                [7,9,11],
                [8,10,12],
            ],$dtype);
            $C = $this->zeros([4,4],$dtype);

            [
                $order,$uplo,$trans,
                $N,$K,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events,
            ] = $this->translate_syr2k($A,$B,null,null,$C);

            $blas->syr2k(
                $order,
                $uplo,$trans,
                $N,$K,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events
            );

            $this->assertEquals([
                [44, 77,134,167],
                [ 0,128,239,290],
                [ 0,  0,440,545],
                [ 0,  0,  0,668]
            ],$C->toArray());

            /*
            // complex128
            $dtype = NDArray::complex128;
            $A = $this->array($this->toComplex([
                [1,2,3],
                [4,5,6],
                [7,8,9],
                [10,11,12],
            ]),$dtype);
            $B = $this->array($this->toComplex([
                [1,3,5],
                [2,4,6],
                [7,9,11],
                [8,10,12],
            ]),$dtype);
            $C = $this->zeros([4,4],$dtype);

            [
                $order,$uplo,$trans,
                $N,$K,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events,
            ] = $this->translate_syr2k($A,$B,null,null,$C);

            $blas->syr2k(
                $order,
                $uplo,$trans,
                $N,$K,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $beta,
                $CC,$offC,$ldc,
                $queue,$events,
            );

            $this->assertEquals($this->toComplex([
                [44, 77,134,167],
                [ 0,128,239,290],
                [ 0,  0,440,545],
                [ 0,  0,  0,668]
            ]),$this->toComplex($C->toArray()));
            */

        }
    }

    //
    // trmm
    //

    public function translate_trmm(
        NDArray $A,
        NDArray $B,
        $alpha=null,
        bool $right=null,
        bool $lower=null,
        bool $trans=null,
        bool $conj=null,
        bool $unit=null,
        object $events=null
        ) : array
    {
        [$trans,$conj] = $this->complementTrans($trans,$conj,$A->dtype());

        if($A->ndim()!=2 || $B->ndim()!=2) {
            throw new InvalidArgumentException('Dimensions must be 2D-NDArray');
        }
        $shapeA = $A->shape();
        $shapeB = $B->shape();
        if($shapeA[0]!=$shapeA[1]) {
            throw new InvalidArgumentException('Matrix A must be square.: '.
                '['.implode(',',$shapeA).']');
        }
        if($right) {
            $sizeA = $shapeB[1];
        } else {
            $sizeA = $shapeB[0];
        }
        if($sizeA!=$shapeA[0]) {
            throw new InvalidArgumentException('Unmatch shape of Matrix A and B: '.
                '['.implode(',',$shapeA).'] <=> ['.implode(',',$shapeB).']');
        }
        $AA   = $A->buffer();
        $offA = $A->offset();
        $BB   = $B->buffer();
        $offB = $B->offset();
        $M = $shapeB[0];
        $N = $shapeB[1];

        if($alpha===null) {
            //if($this->isComplex($A->dtype())) {
            //    $alpha = C(1.0);
            //} else {
                $alpha = 1.0;
            //}
        }

        $lda = ($right) ? $N : $M;
        $ldb = $N;
        $side  = ($right) ? BLAS::Right : BLAS::Left;
        $uplo  = ($lower) ? BLAS::Lower : BLAS::Upper;
        $trans = $this->transToCode($trans,$conj);
        $diag  = ($unit)  ? BLAS::Unit  : BLAS::NonUnit;
        $order = BLAS::RowMajor;
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }

        return [
            $order,$side,$uplo,$trans,$diag,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $this->queue,$events,
        ];
    }

    public function testTrmmNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $A = $this->array([
            [1,2,3],
            [9,4,5],
            [9,9,6],
        ],$dtype);
        $B = $this->array([
            [1, 2, 3, 4],
            [5, 6, 7, 8],
            [9,10,11,12],
        ],$dtype);

        [
            $order,$side,$uplo,$trans,$diag,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $queue,$events,
        ] = $this->translate_trmm($A,$B);

        $blas->trmm(
            $order,$side,$uplo,$trans,$diag,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $queue,$events
        );

        $this->assertEquals([
            [ 38, 44, 50, 56],
            [ 65, 74, 83, 92],
            [ 54, 60, 66, 72]
        ],$B->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [1,2,3],
            [9,4,5],
            [9,9,6],
        ]),$dtype);
        $B = $this->array($this->toComplex([
            [1, 2, 3, 4],
            [5, 6, 7, 8],
            [9,10,11,12],
        ]),$dtype);

        [
            $order,$side,$uplo,$trans,$diag,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $queue,$events,
        ] = $this->translate_trmm($A,$B);

        $blas->trmm(
            $order,$side,$uplo,$trans,$diag,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $queue,$events,
        );

        $this->assertEquals($this->toComplex([
            [ 38, 44, 50, 56],
            [ 65, 74, 83, 92],
            [ 54, 60, 66, 72]
        ]),$this->toComplex($B->toArray()));

        // complex64 check imag
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [C(1,i:6),C(2,i:5),C(3,i:4)],
            [C(9,i:0),C(4,i:3),C(5,i:2)],
            [C(9,i:0),C(9,i:0),C(6,i:1)],
        ]),$dtype);
        $B = $this->array($this->toComplex([
            [1, 2, 3, 4],
            [5, 6, 7, 8],
            [9,10,11,12],
        ]),$dtype);

        [
            $order,$side,$uplo,$trans,$diag,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $queue,$events,
        ] = $this->translate_trmm($A,$B);

        $blas->trmm(
            $order,$side,$uplo,$trans,$diag,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $queue,$events,
        );

        $this->assertEquals($this->toComplex([
            [C(38,i:67),C(44,i:82),C(50,i:97),C(56,i:112)],
            [C(65,i:33),C(74,i:38),C(83,i:43),C(92,i: 48)],
            [C(54,i: 9),C(60,i:10),C(66,i:11),C(72,i: 12)]
        ]),$this->toComplex($B->toArray()));
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $A = $this->array([
                [1,2,3],
                [9,4,5],
                [9,9,6],
            ],$dtype);
            $B = $this->array([
                [1, 2, 3, 4],
                [5, 6, 7, 8],
                [9,10,11,12],
            ],$dtype);

            [
                $order,$side,$uplo,$trans,$diag,
                $M,$N,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $queue,$events,
            ] = $this->translate_trmm($A,$B);

            $blas->trmm(
                $order,$side,$uplo,$trans,$diag,
                $M,$N,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $queue,$events
            );

            $this->assertEquals([
                [ 38, 44, 50, 56],
                [ 65, 74, 83, 92],
                [ 54, 60, 66, 72]
            ],$B->toArray());

            /*
            // complex128
            $dtype = NDArray::complex128;
            $A = $this->array($this->toComplex([
                [1,2,3],
                [9,4,5],
                [9,9,6],
            ]),$dtype);
            $B = $this->array($this->toComplex([
                [1, 2, 3, 4],
                [5, 6, 7, 8],
                [9,10,11,12],
            ]),$dtype);

            [
                $order,$side,$uplo,$trans,$diag,
                $M,$N,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $queue,$events,
            ] = $this->translate_trmm($A,$B);

            $blas->trmm(
                $order,$side,$uplo,$trans,$diag,
                $M,$N,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $queue,$events,
            );

            $this->assertEquals($this->toComplex([
                [ 38, 44, 50, 56],
                [ 65, 74, 83, 92],
                [ 54, 60, 66, 72]
            ]),$this->toComplex($B->toArray()));
            */

        }
    }

    //
    // trsm
    //

    public function translate_trsm(
        NDArray $A,
        NDArray $B,
        $alpha=null,
        bool $right=null,
        bool $lower=null,
        bool $trans=null,
        bool $conj=null,
        bool $unit=null,
        object $events=null
        ) : array
    {
        [$trans,$conj] = $this->complementTrans($trans,$conj,$A->dtype());

        if($A->ndim()!=2 || $B->ndim()!=2) {
            throw new InvalidArgumentException('Dimensions must be 2D-NDArray');
        }
        $shapeA = $A->shape();
        $shapeB = $B->shape();
        if($right) {
            $sizeA = $shapeB[1];
        } else {
            $sizeA = $shapeB[0];
        }
        if($sizeA!=$shapeA[0]) {
            throw new InvalidArgumentException('Unmatch shape of Matrix A and B: '.
                '['.implode(',',$shapeA).'] <=> ['.implode(',',$shapeA).']');
        }
        $AA   = $A->buffer();
        $offA = $A->offset();
        $BB   = $B->buffer();
        $offB = $B->offset();
        $M = $shapeB[0];
        $N = $shapeB[1];

        if($alpha===null) {
            //if($this->isComplex($A->dtype())) {
            //    $alpha = C(1.0);
            //} else {
                $alpha = 1.0;
            //}
        }

        $lda = ($right) ? $N : $M;
        $ldb = $N;
        $side  = ($right) ? BLAS::Right : BLAS::Left;
        $uplo  = ($lower) ? BLAS::Lower : BLAS::Upper;
        $diag  = ($unit)  ? BLAS::Unit  : BLAS::NonUnit;
        $trans = $this->transToCode($trans,$conj);
        $order = BLAS::RowMajor;
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }

        return [
            $order,$side,$uplo,$trans,$diag,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $this->queue,$events,
        ];

    }

    public function testTrsmNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $A = $this->array([
            [1,2,3],
            [9,4,5],
            [9,9,6],
        ],$dtype);
        $B = $this->array([
            [ 7, 8],
            [10,11],
            [13,14],
        ],$dtype);
        [
            $order,$side,$uplo,$trans,$diag,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $queue,$events,
        ] = $this->translate_trsm($A,$B);
        $origB = $this->zeros($B->shape(),$B->dtype());
        //$blas->copy(count($BB),$BB,0,1,$origB->buffer(),0,1);
        $this->copy($B,$origB);

        $blas->trsm(
            $order,$side,$uplo,$trans,$diag,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $queue,$events
        );
        $RA = $this->array([
            [1,2,3],
            [0,4,5],
            [0,0,6],
        ],$A->dtype());
        $C = $this->zeros($B->shape(),$B->dtype());
        $blas->gemm(
            BLAS::RowMajor,$trans,BLAS::NoTrans,
            $M,$N,$M,
            1.0,
            $RA->buffer(),0,$M,
            $BB,$offB,$ldb,
            0.0,
            $C->buffer(),0,$N,
            $queue,$events
        );

        $this->assertTrue($this->isclose($C,$origB));

        /*
        // complex64
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [1,2,3],
            [9,4,5],
            [9,9,6],
        ]),$dtype);
        $B = $this->array($this->toComplex([
            [ 7, 8],
            [10,11],
            [13,14],
        ]),$dtype);
        [
            $order,$side,$uplo,$trans,$diag,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $queue,$events,
        ] = $this->translate_trsm($A,$B);
        $origB = $this->zeros($B->shape(),$B->dtype());
        //$blas->copy(count($BB),$BB,0,1,$origB->buffer(),0,1);
        $this->copy($B,$origB);

        $blas->trsm(
            $order,$side,$uplo,$trans,$diag,
            $M,$N,
            $alpha,
            $AA,$offA,$lda,
            $BB,$offB,$ldb,
            $queue,$events,
        );
        $RA = $this->array($this->toComplex([
            [1,2,3],
            [0,4,5],
            [0,0,6],
        ]),$A->dtype());
        $C = $this->zeros($B->shape(),$B->dtype());
        $blas->gemm(
            BLAS::RowMajor,$trans,BLAS::NoTrans,
            $M,$N,$M,
            C(1.0),
            $RA->buffer(),0,$M,
            $BB,$offB,$ldb,
            C(0.0),
            $C->buffer(),0,$N,
            $queue,$events,
        );

        //echo "=====C=====\n";
        //$CC = $C->buffer();
        //$len = count($CC);
        //for($i=0;$i<$len;++$i) {
        //    echo $CC[$i]->real.','.$CC[$i]->imag."\n";
        //}

        $this->assertTrue($this->isclose($C,$origB,atol:1e-6));
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $A = $this->array([
                [1,2,3],
                [9,4,5],
                [9,9,6],
            ],$dtype);
            $B = $this->array([
                [ 7, 8],
                [10,11],
                [13,14],
            ],$dtype);
            [
                $order,$side,$uplo,$trans,$diag,
                $M,$N,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $queue,$events,
            ] = $this->translate_trsm($A,$B);
            $origB = $this->zeros($B->shape(),$B->dtype());
            //$blas->copy(count($BB),$BB,0,1,$origB->buffer(),0,1);
            $this->copy($B,$origB);

            $blas->trsm(
                $order,$side,$uplo,$trans,$diag,
                $M,$N,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $queue,$events
            );
            $RA = $this->array([
                [1,2,3],
                [0,4,5],
                [0,0,6],
            ],$A->dtype());
            $C = $this->zeros($B->shape(),$B->dtype());
            $blas->gemm(
                BLAS::RowMajor,$trans,BLAS::NoTrans,
                $M,$N,$M,
                1.0,
                $RA->buffer(),0,$M,
                $BB,$offB,$ldb,
                0.0,
                $C->buffer(),0,$N,
                $queue,$events
            );

            $this->assertTrue($this->isclose($C,$origB));

            /*
            // complex128
            $dtype = NDArray::complex128;
            $A = $this->array($this->toComplex([
                [1,2,3],
                [9,4,5],
                [9,9,6],
            ]),$dtype);
            $B = $this->array($this->toComplex([
                [ 7, 8],
                [10,11],
                [13,14],
            ]),$dtype);
            [
                $order,$side,$uplo,$trans,$diag,
                $M,$N,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $queue,$events,
            ] = $this->translate_trsm($A,$B);
            $origB = $this->zeros($B->shape(),$B->dtype());
            //$blas->copy(count($BB),$BB,0,1,$origB->buffer(),0,1);
            $this->copy($B,$origB);

            $blas->trsm(
                $order,$side,$uplo,$trans,$diag,
                $M,$N,
                $alpha,
                $AA,$offA,$lda,
                $BB,$offB,$ldb,
                $queue,$events,
            );
            $RA = $this->array($this->toComplex([
                [1,2,3],
                [0,4,5],
                [0,0,6],
            ]),$A->dtype());
            $C = $this->zeros($B->shape(),$B->dtype());
            $blas->gemm(
                BLAS::RowMajor,$trans,BLAS::NoTrans,
                $M,$N,$M,
                C(1.0),
                $RA->buffer(),0,$M,
                $BB,$offB,$ldb,
                C(0.0),
                $C->buffer(),0,$N,
                $queue,$events,
            );

            //echo "=====C=====\n";
            //$CC = $C->buffer();
            //$len = count($CC);
            //for($i=0;$i<$len;++$i) {
            //    echo $CC[$i]->real.','.$CC[$i]->imag."\n";
            //}

            $this->assertTrue($this->isclose($C,$origB,atol:1e-6));
            */
        }
    }
    
    //
    // omatcopy  - hadamard
    //

    public function translate_omatcopy(
        NDArray $A,
        bool $trans=null,
        $alpha=null,
        NDArray $B=null,
        bool $conj=null,
        object $events=null
        ) : array
    {
        [$trans,$conj] = $this->complementTrans($trans,$conj,$A->dtype());

        if($A->ndim()!=2 || $B->ndim()!=2) {
            throw new InvalidArgumentException('Dimensions must be 2D-NDArray');
        }
        [$rows,$cols] = $A->shape();
        if($trans) {
            [$rows,$cols] = [$cols,$rows];
        }
        if($B->shape()!=[$rows,$cols]) {
            $shapeError = '('.implode(',',$A->shape()).'),('.implode(',',$B->shape()).')';
            throw new InvalidArgumentException("Unmatch shape of dimension: ".$shapeError);
        }

        [$M,$N] = $A->shape();
        if($alpha===null) {
            //if($this->isComplex($A->dtype())) {
            //    $alpha = C(1.0);
            //} else {
                $alpha = 1.0;
            //}
        }
        $AA = $A->buffer();
        $offA = $A->offset();
        $ldA = $N;
        $BB = $B->buffer();
        $offB = $B->offset();
        $ldB = $cols;

        $trans = $this->transToCode($trans,$conj);
        $order = BLAS::RowMajor;
        if($events==null) {
            $events = $this->getOpenCL()->EventList();
        }

        return [
            $order,$trans,
            $M,$N,
            $alpha,
            $AA, $offA, $ldA,
            $BB, $offB, $ldB,
            $this->queue,$events,
        ];
    }


    public function testOmatcopyNormal()
    {
        $blas = $this->getBlas();

        // float32
        $dtype = NDArray::float32;
        $A = $this->array([
            [1,2,3],
            [4,5,6],
            [7,8,9],
            [10,11,12],
        ],$dtype);
        $B = $this->zeros([3,4],$dtype);

        [
            $order,$trans,
            $M,$N,
            $alpha,
            $AA, $offA, $ldA,
            $BB, $offB, $ldB,
            $queue,$events,
        ] = $this->translate_omatcopy($A,true,null,$B);  // trans:true

        $blas->omatcopy(
            $order,$trans,
            $M,$N,
            $alpha,
            $AA, $offA, $ldA,
            $BB, $offB, $ldB,
            $queue,$events
        );

        $this->assertEquals([
            [1,4,7,10],
            [2,5,8,11],
            [3,6,9,12],
        ],$B->toArray());

        /*
        // complex64 trans and conj
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [C(1,i:100),C(2,i:200),3],
            [4,5,6],
            [7,8,9],
            [10,11,12],
        ]),$dtype);
        $B = $this->zeros([3,4],$dtype);

        [
            $order,$trans,
            $M,$N,
            $alpha,
            $AA, $offA, $ldA,
            $BB, $offB, $ldB,
            $queue,$events,
        ] = $this->translate_omatcopy($A,true,null,$B); // trans:true

        $blas->omatcopy(
            $order,$trans,
            $M,$N,
            $alpha,
            $AA, $offA, $ldA,
            $BB, $offB, $ldB,
            $queue,$events,
        );

        $this->assertEquals($this->toComplex([
            [C(1,i:-100),4,7,10],
            [C(2,i:-200),5,8,11],
            [3,6,9,12],
        ]),$this->toComplex($B->toArray()));

        // complex64  trans and no_conj
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [C(1,i:100),C(2,i:200),3],
            [4,5,6],
            [7,8,9],
            [10,11,12],
        ]),$dtype);
        $B = $this->zeros([3,4],$dtype);

        [
            $order,$trans,
            $M,$N,
            $alpha,
            $AA, $offA, $ldA,
            $BB, $offB, $ldB,
            $queue,$events,
        ] = $this->translate_omatcopy($A,true,null,$B,false); // trans:true conj:false
        $blas->omatcopy(
            $order,$trans,
            $M,$N,
            $alpha,
            $AA, $offA, $ldA,
            $BB, $offB, $ldB,
            $queue,$events,
        );

        $this->assertEquals($this->toComplex([
            [C(1,i:100),4,7,10],
            [C(2,i:200),5,8,11],
            [3,6,9,12],
        ]),$this->toComplex($B->toArray()));


        // *** CAUTION ***
        // CLBlast does not support ConjNoTrans
        //
        // complex64  no_trans and conj
        //$A = $this->array($this->toComplex([
        //    [C(1,i:100),C(2,i:200),3],
        //    [4,5,6],
        //    [7,8,9],
        //    [10,11,12],
        //]),NDArray::complex64);
        //$B = $this->zeros([4,3],NDArray::complex64);
        //
        //[
        //    $order,$trans,
        //    $M,$N,
        //    $alpha,
        //    $AA, $offA, $ldA,
        //    $BB, $offB, $ldB,
        //    $queue,$events,
        //] = $this->translate_omatcopy($A,false,null,$B,true); // trans:false conj:true 
        //$blas->omatcopy(
        //    $order,$trans,
        //    $M,$N,
        //    $alpha,
        //    $AA, $offA, $ldA,
        //    $BB, $offB, $ldB,
        //    $queue,$events,
        //);
        //
        //$this->assertEquals($this->toComplex([
        //    [C(1,i:-100),C(2,i:-200),3],
        //    [4,5,6],
        //    [7,8,9],
        //    [10,11,12],
        //]),$this->toComplex($B->toArray()));
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $A = $this->array([
                [1,2,3],
                [4,5,6],
                [7,8,9],
                [10,11,12],
            ],$dtype);
            $B = $this->zeros([3,4],$dtype);

            [
                $order,$trans,
                $M,$N,
                $alpha,
                $AA, $offA, $ldA,
                $BB, $offB, $ldB,
                $queue,$events,
            ] = $this->translate_omatcopy($A,true,null,$B); // trans:true

            $blas->omatcopy(
                $order,$trans,
                $M,$N,
                $alpha,
                $AA, $offA, $ldA,
                $BB, $offB, $ldB,
                $queue,$events
            );

            $this->assertEquals([
                [1,4,7,10],
                [2,5,8,11],
                [3,6,9,12],
            ],$B->toArray());

            /*
            // complex128 trans and conj
            $dtype = NDArray::complex128;
            $A = $this->array($this->toComplex([
                [C(1,i:100),C(2,i:200),3],
                [4,5,6],
                [7,8,9],
                [10,11,12],
            ]),$dtype);
            $B = $this->zeros([3,4],$dtype);
        
            [
                $order,$trans,
                $M,$N,
                $alpha,
                $AA, $offA, $ldA,
                $BB, $offB, $ldB,
                $queue,$events,
            ] = $this->translate_omatcopy($A,true,null,$B); // trans:true
            
            $blas->omatcopy(
                $order,$trans,
                $M,$N,
                $alpha,
                $AA, $offA, $ldA,
                $BB, $offB, $ldB,
                $queue,$events,
            );
        
            $this->assertEquals($this->toComplex([
                [C(1,i:-100),4,7,10],
                [C(2,i:-200),5,8,11],
                [3,6,9,12],
            ]),$this->toComplex($B->toArray()));
            */

        }
    }

    protected function getomatcopyTestEnv($m,$n)
    {
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $hostBufferA = $this->newHostBuffer($n*$m,NDArray::float32);
        $hostBufferB = $this->newHostBuffer($m*$n,NDArray::float32);
        $testTruesR = $this->newHostBuffer($m*$n,NDArray::float32);
        $alpha = 2.0;
        for($i=0;$i<$m*$n;$i++) {
            $hostBufferA[$i]=$i;
        }
        for($i=0;$i<$n;$i++) {
            for($j=0;$j<$m;$j++) {
                $hostBufferB[$i*$m+$j]=10000;
                $testTruesR[$i*$m+$j]=$alpha*($j*$n+$i);
            }
        }

        $bufferA = $ocl->Buffer($context,intval($m*$n*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferA);
        $bufferB = $ocl->Buffer($context,intval($m*$n*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferB);

        $blas = $this->getBlas();
        $events = $ocl->EventList();
        return [
            $queue,$blas,$events,$bufferA,$bufferB,
            $hostBufferA,$hostBufferB,$testTruesR,$alpha,
        ];
    }

    public function testOmatcopyFullRange()
    {
        $m=256;
        $n=128;
        [
            $queue,$blas,$events,$bufferA,$bufferB,
            $hostBufferA,$hostBufferB,$testTruesR,$alpha,
        ] = $this->getomatcopyTestEnv($m,$n);
        $blas->omatcopy(BLAS::RowMajor,BLAS::Trans,$m,$n,$alpha,
            $bufferA,$offsetA=0,$ldA=$n,
            $bufferB,$offsetB=0,$ldB=$m,
            $queue,$events);
        $events->wait();
        $bufferB->read($queue,$hostBufferB,intval($m*$n*32/8));
        $equals = true;
        for($i=0;$i<$n;$i++) {
            for($j=0;$j<$m;$j++) {
                if($hostBufferB[$i*$m+$j]!=$testTruesR[$i*$m+$j]) {
                    $equals = false;
                    break;
                }
            }
            if($equals==false) {
                break;
            }
        }
        $this->assertTrue($equals);
    }

    public function testOmatcopyInvalidBufferAObject()
    {
        $m=256;
        $n=128;
        [
            $queue,$blas,$events,$bufferA,$bufferB,
            $hostBufferA,$hostBufferB,$testTruesR,$alpha,
        ] = $this->getomatcopyTestEnv($m,$n);
         // invalid buffer object arguments
         $bufferA = new \stdClass();
         $this->expectException(TypeError::class);
         //$this->expectExceptionMessage('??????????????????');
        $blas->omatcopy(BLAS::RowMajor,BLAS::Trans,$m,$n,$alpha,
            $bufferA,$offsetA=0,$ldA=$n,
            $bufferB,$offsetB=0,$ldB=$m,
            $queue,$events);
    }

    public function testOmatcopyInvalidBufferBObject()
    {
        $m=256;
        $n=128;
        [
            $queue,$blas,$events,$bufferA,$bufferB,
            $hostBufferA,$hostBufferB,$testTruesR,$alpha,
        ] = $this->getomatcopyTestEnv($m,$n);
         // invalid buffer object arguments
         $bufferB = new \stdClass();
         $this->expectException(TypeError::class);
         //$this->expectExceptionMessage('??????????????????');
        $blas->omatcopy(BLAS::RowMajor,BLAS::Trans,$m,$n,$alpha,
            $bufferA,$offsetA=0,$ldA=$n,
            $bufferB,$offsetB=0,$ldB=$m,
            $queue,$events);
    }

    public function testOmatcopyInvalidQueueObject()
    {
        $m=256;
        $n=128;
        [
            $queue,$blas,$events,$bufferA,$bufferB,
            $hostBufferA,$hostBufferB,$testTruesR,$alpha,
        ] = $this->getomatcopyTestEnv($m,$n);
         // invalid Queue object arguments
         $queue = new \stdClass();
         $this->expectException(TypeError::class);
         //$this->expectExceptionMessage('??????????????????');
        $blas->omatcopy(BLAS::RowMajor,BLAS::Trans,$m,$n,$alpha,
            $bufferA,$offsetA=0,$ldA=$n,
            $bufferB,$offsetB=0,$ldB=$m,
            $queue,$events);
    }

    public function testOmatcopyInvalidEventsObject()
    {
        $m=256;
        $n=128;
        [
            $queue,$blas,$events,$bufferA,$bufferB,
            $hostBufferA,$hostBufferB,$testTruesR,$alpha,
        ] = $this->getomatcopyTestEnv($m,$n);
         // invalid Events object arguments
         $events = new \stdClass();
         $this->expectException(TypeError::class);
         //$this->expectExceptionMessage('??????????????????');
        $blas->omatcopy(BLAS::RowMajor,BLAS::Trans,$m,$n,$alpha,
            $bufferA,$offsetA=0,$ldA=$n,
            $bufferB,$offsetB=0,$ldB=$m,
            $queue,$events);
    }

}
