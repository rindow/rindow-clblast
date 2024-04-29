<?php
namespace RindowTest\CLBlast;

class NDArrayPhp extends AbstractNDArrayPhp
{
    public function offsetGet( $offset ) : mixed
    {
        return $this->doOffsetGet( $offset );
    }
}