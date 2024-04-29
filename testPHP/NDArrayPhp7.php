<?php
namespace RindowTest\CLBlast;

class NDArrayPhp extends AbstractNDArrayPhp
{
    public function offsetGet( $offset )
    {
        return $this->doOffsetGet( $offset );
    }
}