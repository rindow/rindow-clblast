<?php
namespace RindowTest\CLBlast;

use Interop\Polite\Math\Matrix\NDArray;
use Interop\Polite\Math\Matrix\BLAS;
use Interop\Polite\Math\Matrix\OpenCL;
use Interop\Polite\Math\Matrix\DeviceBuffer;
use Interop\Polite\Math\Matrix\LinearBuffer;
use InvalidArgumentException;
use RuntimeException;
use TypeError;
use ArrayObject;
use ArrayAccess;
use RindowTest\CLBlast\OpenCLFactory;
use Rindow\OpenCL\EventList;

include_once __DIR__.'/../testPHP/HostBuffer.php';
require_once __DIR__.'/AbstractNDArrayPhp.php';
require_once __DIR__.'/AbstractNDArrayCL.php';
if(version_compare(phpversion(),'8.0.0','<')) {
    require_once __DIR__.'/../testPHP/NDArrayPhp7.php';
    require_once __DIR__.'/../testPHP/NDArrayCL7.php';
} else {
    require_once __DIR__.'/../testPHP/NDArrayPhp8.php';
    require_once __DIR__.'/../testPHP/NDArrayCL8.php';
}

function C(
    float $r=null,
    float $i=null
) : object
{
    if($r===null) {
        $r = 0.0;
    }
    if($i===null) {
        $i = 0.0;
    }
    return (object)['real'=>$r,'imag'=>$i];
}

trait Utils
{
    protected $openCLFactory = null;
    protected $queue;
    protected $fp64 = null;

    public function setOpenCLQueue(object $openCLQueue)
    {
        $this->queue = $openCLQueue;
    }

    protected function fp64() : bool
    {
        if($this->fp64!==null) {
            return $this->fp64;
        }
        $devices = $this->queue->getContext()->getInfo(OpenCL::CL_CONTEXT_DEVICES);
        $extensions = $devices->getInfo(0,OpenCL::CL_DEVICE_EXTENSIONS);
        if(strpos($extensions,'cl_khr_fp64')===false) {
            $this->fp64 = false;
        } else {
            $this->fp64 = true;
        }
        return $this->fp64;
    }

    public function getOpenCL()
    {
        if($this->opencl===null) {
            $this->opencl = new OpenCLFactory();
        }
        return $this->opencl;
    }

    //public function openCLFactory() : object
    //{
    //    if($this->openCLFactory==null) {
    //        $this->openCLFactory = new OpenCLFactory();
    //    }
    //    return $this->openCLFactory;
    //}

    public function hostArray($array=null, int $dtype=null, array $shape=null) : object
    {
        $ndarray = new NDArrayPhp($array, $dtype, $shape);
        return $ndarray;
    }

    public function alloc(array $shape,int $dtype=null, int $flags=null)
    {
        $ndarray = $this->hostArray(null,$dtype,$shape);
        return $this->array($ndarray, $dtype, $flags);
    }

    public function zeros(array $shape,int $dtype=null, int $flags=null)
    {
        $ndarray = $this->hostArray(null,$dtype,$shape);
        return $this->array($ndarray, $dtype, $flags);
    }

    public function zerosLike(NDArray $x,int $dtype=null, int $flags=null)
    {
        // argument $dtype is dummy
        return $this->zeros($x->shape(),$x->dtype(), $flags);
    }

    public function ones(array $shape, int $dtype=null, int $flags=null)
    {
        $ndarray = $this->hostArray(null,$dtype,$shape);
        $buffer = $ndarray->buffer();
        $dtype = $ndarray->dtype();
        $value = 1;
        if($this->isComplex($dtype)) {
            $value = $this->toComplex($value);
        }
        $size = count($buffer);
        for($i=0;$i<$size;++$i) {
            $buffer[$i] = $value;
        }
        return $this->array($ndarray, $dtype, $flags);
    }

    public function full(array $shape, $value ,int $dtype=null, int $flags=null)
    {
        $ndarray = $this->hostArray(null,$dtype,$shape);
        $buffer = $ndarray->buffer();
        $size = count($buffer);
        for($i=0;$i<$size;++$i) {
            $buffer[$i] = $value;
        }
        return $this->array($ndarray, $dtype, $flags);
    }

    public function isComplex($dtype) : bool
    {
        return $dtype==NDArray::complex64||$dtype==NDArray::complex128;
    }

    public function toComplex($array) : mixed
    {
        if(!is_array($array)) {
            if(is_numeric($array)) {
                return C($array,0);
            } else {
                return C($array->real,$array->imag);
            }
        }
        $cArray = [];
        foreach($array as $value) {
            $cArray[] = $this->toComplex($value);
        }
        return $cArray;
    }

    protected function buildValByType($value, int $dtype)
    {
        if($this->isComplex($dtype)) {
            $value = $this->toComplex($value);
        }
        return $value;
    }

    protected function complementTrans(?bool $trans,?bool $conj,int $dtype) : array
    {
        $trans = $trans ?? false;
        if($this->isComplex($dtype)) {
            $conj = $conj ?? $trans;
        } else {
            $conj = $conj ?? false;
        }
        return [$trans,$conj];
    }

    protected function transToCode(bool $trans,bool $conj) : int
    {
        if($trans) {
            return $conj ? BLAS::ConjTrans : BLAS::Trans;
        } else {
            return $conj ? BLAS::ConjNoTrans : BLAS::NoTrans;
        }
    }

    protected function abs($value) : float
    {
        if(is_numeric($value)) {
            return abs($value);
        }
        $abs = sqrt(($value->real)**2+($value->imag)**2);
        return $abs;
    }

    protected function copy(NDArray $x,NDArray $y=null) : NDArray
    {
        $blas = $this->getBlas();

        if($y==null) {
            $y = $this->zeros($x->shape(),$x->dtype());
        }
        [$N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events] = $this->translate_copy($x,$y);
        $blas->copy($N,$XX,$offX,$incX,$YY,$offY,$incY,$queue,$events);
        $events->wait();
        return $y;
    }

    protected function axpy(NDArray $x,NDArray $y=null,$alpha=null) : NDArray
    {
        $blas = $this->getBlas();

        if($y==null) {
            $y = $this->zeros($x->shape(),$x->dtype());
        }
        if($alpha===null) {
            $alpha = 1.0;
        }
        $N = $x->size();
        $XX = $x->buffer();
        $offX = $x->offset();
        $YY = $y->buffer();
        $offY = $y->offset();

        $events = $this->getOpenCL()->EventList();

        $blas->axpy($N,$alpha,$XX,$offX,1,$YY,$offY,1,$this->queue,$events);
        $events->wait();
        return $y;
    }

    protected function iamax(
        NDArray $x,
        NDArray $output=null
    ) : NDArray
    {
        $blas = $this->getBlas();

        $r = $output;
        if($r==null) {
            // *** CAUTION ****
            // Index result is 32bit
            $r = $this->alloc([],NDArray::int32,OpenCL::CL_MEM_READ_WRITE);
        }
        $N = $x->size();
        $RR = $r->buffer();
        $offR = $r->offset();
        $XX = $x->buffer();
        $offX = $x->offset();
        $events = $this->getOpenCL()->EventList();

        $blas->iamax($N,$RR,$offR,$XX,$offX,1,$this->queue,$events);
        $events->wait();
        return $r;
    }

    protected function scal(float $a,NDArray $x) : NDArray
    {
        $blas = $this->getBlas();

        $N = $x->size();
        $XX = $x->buffer();
        $offX = $x->offset();
        $events = $this->getOpenCL()->EventList();

        $blas->scal($N,$a,$XX,$offX,1,$this->queue,$events);
        $events->wait();
        return $x;
    }

    protected function isclose(NDArray $a, NDArray $b, $rtol=null, $atol=null) : bool
    {
        $blas = $this->getBlas();

        $isCpx = $this->isComplex($a->dtype());
        if($rtol===null) {
            $rtol = $isCpx?C(1e-04):1e-04;
        }
        if($atol===null) {
            $atol = 1e-07;
        }
        if($a->shape()!=$b->shape()) {
            return false;
        }
        // diff = b - a
        $alpha =  $isCpx?C(-1):-1;
        $diffs = $this->copy($b);
        $this->axpy($a,$diffs,$alpha);
        $iDiffMax = $this->iamax($diffs);
        $diff = $this->abs($diffs->toNDArray()->buffer()[$iDiffMax->toArray()]);

        // close = atol + rtol * b
        $scalB = $this->copy($b);
        $this->scal($rtol,$scalB);
        $iCloseMax = $this->iamax($scalB);
        $close = $atol+$this->abs($scalB->toNDArray()->buffer()[$iCloseMax->toArray()]);

        return $diff < $close;
    }

    public function NDArrayCL(
        object $queue, $buffer=null, int $dtype=null, array $shape = null,
        int $offset=null, int $flags=null
    ) : NDArray
    {
        $arrayCL = new NDArrayCL(
            $queue, $buffer, $dtype, $shape,
            $offset, $flags
        );
        return $arrayCL;
    }

    public function array($array, int $dtype=null, int $flags=null) : NDArray
    {
        if($array instanceof NDArray) {
            $buffer = $array->buffer();
            if($buffer instanceof LinearBuffer) {
                ;
            } elseif($buffer instanceof DeviceBuffer) {
                return $array;
            } else {
                throw new InvalidArgumentException('Unsuppored buffer type.');
            }
        } elseif(is_array($array) || is_numeric($array)) {
            $array = $this->hostArray($array,$dtype);
        } else {
            throw new InvalidArgumentException('input value must be NDArray or array');
        }
        if($flags==null) {
            $flags = OpenCL::CL_MEM_READ_WRITE;
        }
        $flags = $flags | OpenCL::CL_MEM_COPY_HOST_PTR;

        $arrayCL = $this->NDArrayCL(
            $this->queue,
            $array->buffer(), $array->dtype(), $array->shape(),
            $array->offset(), $flags
        );
        return $arrayCL;
    }
}