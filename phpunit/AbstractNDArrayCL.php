<?php
namespace RindowTest\CLBlast;

use ArrayObject;
use ArrayAccess;
use InvalidArgumentException;
use Interop\Polite\Math\Matrix\NDArray;
use Interop\Polite\Math\Matrix\DeviceBuffer;
use Interop\Polite\Math\Matrix\LinearBuffer;

abstract class AbstractNDArrayCL implements NDArray {
    static protected $valueSizeTable = [
        NDArray::bool  => 1,
        NDArray::int8  => 1,
        NDArray::int16 => 2,
        NDArray::int32 => 4,
        NDArray::int64 => 8,
        NDArray::uint8 => 1,
        NDArray::uint16 => 2,
        NDArray::uint32 => 4,
        NDArray::uint64 => 8,
        NDArray::float8 => 1,
        NDArray::float16 => 2,
        NDArray::float32 => 4,
        NDArray::float64 => 8,
        NDArray::complex16 => 2,
        NDArray::complex32 => 4,
        NDArray::complex64 => 8,
        NDArray::complex128 => 16,
    ];
    protected $buffer;
    protected $size;
    protected $dtype;
    protected $offset;
    protected $shape;
    protected $flags;
    //protected $service;
    protected $context;
    protected $queue;
    
    public function __construct(
        object $queue, $buffer=null, int $dtype=null, array $shape = null,
        int $offset=null, int $flags=null,
        object $service=null)
    {
        //if($service===null) {
        //    throw new InvalidArgumentException("No service specified.");
        //}
        //$this->service = $service;
        //$this->clBufferFactory = $service->buffer(Service::LV_ACCELERATED);
        $context = $queue->getContext();
        $this->context = $context; 
        $this->queue = $queue;
        if($dtype===null) {
            $dtype = NDArray::float32;
        } else {
            $dtype = $dtype;
        }
        if($offset===null) {
            $offset = 0;
        }
        if($flags===null) {
            $flags = OpenCL::CL_MEM_READ_WRITE;
        }

        $this->assertShape($shape);
        $this->shape = $shape;
        $this->flags = $flags;
        $size = (int)array_product($shape);
        if($buffer instanceof DeviceBuffer) {
            if($buffer->bytes()
                < ($size + $offset)*static::$valueSizeTable[$dtype]) {
                throw new InvalidArgumentException("Invalid dimension size");
            }
            $this->dtype  = $dtype;
            $this->buffer = $buffer;
            $this->offset = $offset;
        } elseif($buffer===null) {
            $size = (int)array_product($shape);
            $this->buffer = $this->newBuffer($context,$size,$dtype,$flags);
            $this->dtype  = $dtype;
            $this->offset = 0;
        } elseif($buffer instanceof LinearBuffer) {
            if($offset===null||!is_int($offset)) {
                throw new InvalidArgumentException("Must specify offset with the buffer");
            }
            $size = (int)array_product($shape);
            if($size > count($buffer)-$offset) {
                throw new InvalidArgumentException("host buffer is too small");
            }
            $this->buffer = $this->newBuffer($context,
                    $size, $buffer->dtype(), $flags,
                    $buffer, $offset);
            $this->dtype = $buffer->dtype();
            $this->offset = 0;
        } else {
            if(is_object($buffer)) {
                $typename = get_class($buffer);
            } else {
                $typename = gettype($buffer);
            }
            throw new InvalidArgumentException("Invalid type of array: ".$typename);
        }
    }

    protected function newBuffer(
        object $context, int $size, int $dtype, int $flags=0,
        object $hostBuffer=null, int $hostOffset=0)
    {
        $bytes = static::$valueSizeTable[$dtype]*$size;
        return new OpenCLBuffer($context,$bytes,
            $flags,$hostBuffer,$hostOffset,$dtype);
    }

    protected function assertShape(array $shape)
    {
        foreach($shape as $num) {
            if(!is_int($num)) {
                throw new InvalidArgumentException(
                    "Invalid shape numbers. It gives ".gettype($num));
            }
            if($num<=0) {
                throw new InvalidArgumentException(
                    "Invalid shape numbers. It gives ".$num);
            }
        }
    }

    public function shape() : array
    {
        return $this->shape;
    }

    public function ndim() : int
    {
        return count($this->shape);
    }

    public function dtype()
    {
        return $this->dtype;
    }

    public function flags()
    {
        return $this->flags;
    }

    public function buffer() : ArrayAccess
    {
        return $this->buffer;
    }

    public function offset() : int
    {
        return $this->offset;
    }

    public function valueSize() : int
    {
        return static::$valueSizeTable[$this->dtype];
    }

    public function size() : int
    {
        return (int)array_product($this->shape);
    }

    public function reshape(array $shape) : NDArray
    {
        $this->assertShape($shape);
        if($this->size()!=array_product($shape)) {
            throw new InvalidArgumentException("Unmatch size to reshape: ".
                "[".implode(',',$this->shape())."]=>[".implode(',',$shape)."]");
        }
        $newArray = new static($this->queue,$this->buffer,
            $this->dtype,$shape,$this->offset,$this->flags);
        return $newArray;
    }

    public function toArray()
    {
        $ndarray = $this->toNDArray();
        $array = $ndarray->toArray();
        return $array;
    }

    public function toNDArray(
        bool $blocking_read=null,EventList $waitEvents=null,
        EventList &$events=null) : NDArray
    {
        $blocking_read = $blocking_read ?? true;
        $array = new NDArrayPhp(null,$this->dtype,$this->shape);
        $valueSize = static::$valueSizeTable[$this->dtype];
        $size = array_product($this->shape);
        $bytes = $size*$valueSize; 
        $this->buffer->read(
            $this->queue,
            $array->buffer(),
            $bytes,  // size:
            $this->offset*$valueSize, // offset:
            0, // host_offset:
            $blocking_read, // blocking_read:
            $events, // events:
            $waitEvents // event_wait_list:
        );
        return $array;
    }
    public function offsetExists( $offset ) : bool { throw new \Excpetion('not implement'); }
    protected function doOffsetGet( $offset ) { throw new \Excpetion('not implement'); }
    public function offsetSet( $offset , $value ) : void { throw new \Excpetion('not implement'); }
    public function offsetUnset( $offset ) : void { throw new \Excpetion('not implement'); }
    public function count() : int  { throw new \Excpetion('not implement'); }
    public function  getIterator() : Traversable  { throw new \Excpetion('not implement'); }
}
