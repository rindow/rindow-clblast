<?php
namespace RindowTest\CLBlast;

use Rindow\OpenBLAS\Buffer as BufferImplement;
use Interop\Polite\Math\Matrix\LinearBuffer;

class HostBuffer extends BufferImplement implements LinearBuffer
{
}
