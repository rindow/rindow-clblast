<?php
namespace RindowTest\CLBlast;

use Interop\Polite\Math\Matrix\LinearBuffer;
use Interop\Polite\Math\Matrix\Buffer;
use Rindow\OpenCL\PlatformList;
use Rindow\OpenCL\Context;
use Rindow\OpenCL\CommandQueue;
use Rindow\OpenCL\DeviceList;
use Rindow\OpenCL\Program;
use Rindow\OpenCL\Kernel;
use Rindow\OpenCL\EventList;
use Rindow\Math\Matrix\Drivers\AbstractDriver;
include_once __DIR__.'/OpenCLBuffer.php';

class OpenCLFactory
{
    protected $extName = 'rindow_opencl';

    protected function assertVersion()
    {

    }

    public function PlatformList() : PlatformList
    {
        $this->assertVersion();
        return new PlatformList();
    }

    public function DeviceList(
        PlatformList $platforms,
        int $index=NULL,
        int $deviceType=NULL
    ) : DeviceList
    {
        $this->assertVersion();
        if($index===null) {
            $index = 0;
        }
        if($deviceType===null) {
            $deviceType = 0;
        }
        return new DeviceList($platforms,$index,$deviceType);
    }

    public function Context(
        $arg
    ) : Context
    {
        $this->assertVersion();
        return new Context($arg);
    }

    public function EventList(
        Context $context=null
    ) : EventList
    {
        return new EventList($context);
    }

    public function CommandQueue(
        Context $context,
        int $deviceId=null,
        int $properties=null
    ) : CommandQueue
    {
        if($deviceId===null) {
            $deviceId = 0;
        }
        if($properties===null) {
            $properties = 0;
        }
        return new CommandQueue($context, $deviceId, $properties);
    }

    /**
     * @param string|array<mixed> $source
     */
    public function Program(
        Context $context,
        $source,   // string or list of something
        int $mode=null,         // mode  0:source codes, 1:binary, 2:built-in kernel, 3:linker
        DeviceList $deviceList=null,
        string $options=null
        ) : Program
    {
        if($mode===null) {
            $mode = 0;
        }
        return new Program($context, $source, $mode, $deviceList, $options);
    }

    public function Buffer(
        Context $context,
        int $size,
        int $flags=null,
        LinearBuffer $hostBuffer=null,
        int $hostOffset=null,
        int $dtype=null
        ) : Buffer
    {
        if($hostOffset===null) {
            $hostOffset = 0;
        }
        if($dtype===null) {
            $dtype = 0;
        }
        
        return new OpenCLBuffer($context, $size, $flags, $hostBuffer, $hostOffset, $dtype);
    }

    public function Kernel
    (
        Program $program,
        string $kernelName
        ) : Kernel
    {
        return new Kernel($program, $kernelName);
    }
}
