<?php
namespace RindowTest\CLBlast;

class NDArrayCL extends AbstractNDArrayCL
{
    public function offsetGet( $offset ) : mixed
    {
        return $this->doOffsetGet( $offset );
    }
}