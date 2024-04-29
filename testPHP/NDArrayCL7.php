<?php
namespace RindowTest\CLBlast;

class NDArrayCL extends AbstractNDArrayCL
{
    public function offsetGet( $offset )
    {
        return $this->doOffsetGet( $offset );
    }
}