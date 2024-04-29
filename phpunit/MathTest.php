<?php
namespace RindowTest\CLBlast\MathTest;

use PHPUnit\Framework\TestCase;
use Interop\Polite\Math\Matrix\NDArray;
use Interop\Polite\Math\Matrix\BLAS;
use Interop\Polite\Math\Matrix\OpenCL;

use RindowTest\CLBlast\HostBuffer;
use Rindow\OpenBLAS\Blas as OpenBLAS;
use Rindow\CLBlast\Math;
use Rindow\Matlib\FFI\MatlibFactory;
use InvalidArgumentException;
use RuntimeException;
use TypeError;
use ArrayObject;
use ArrayAccess;

require_once __DIR__.'/OpenCLFactory.php';
require_once __DIR__.'/Utils.php';
use RindowTest\CLBlast\Utils;
use function RindowTest\CLBlast\FFI\C;

class MathTest extends TestCase
{
    use Utils;

    protected $skipDisplayInfo = true;

    //protected $defaultDeviceType = OpenCL::CL_DEVICE_TYPE_DEFAULT;
    //protected $defaultDeviceType = OpenCL::CL_DEVICE_TYPE_GPU;
    static protected $default_device_type = OpenCL::CL_DEVICE_TYPE_GPU;

    protected $opencl=null;
    protected $clblast=null;
    protected $openblas=null;
    protected $matlib=null;

    public function setUp() : void
    {
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $this->setOpenCLQueue($queue);
    }

    public function getMath()
    {
        return new Math();
    }

    public function getOpenBLAS()
    {
        return new OpenBLAS();
    }

    public function getMatlib()
    {
        if($this->matlib===null) {
            $this->matlib = new MatlibFactory();
        }
        return $this->matlib->Math();
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

    public function getCLBlastVersion($math)
    {
        $config = $math->getConfig();
        if(strpos($config,'OpenBLAS')===0) {
            $config = explode(' ',$config);
            return $config[1];
        } else {
            return '0.0.0';
        }
    }

    //
    //  sum
    //

    protected function getSumTestEnv($NMITEM)
    {
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $hostBufferX = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferR = $this->newHostBuffer(1,NDArray::float32);
        $sum = 0.0;
        for($i=0;$i<$NMITEM;$i++) {
            $hostBufferX[$i]=$i/$NMITEM;
            $sum += abs($hostBufferX[$i]);
        }
        $bufferX = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferR = $ocl->Buffer($context,intval(32/8),
            OpenCL::CL_MEM_WRITE_ONLY);
        $math = $this->getMath();
        $events = $ocl->EventList();
        return [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$sum];
    }

    public function testSumNormal()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$sum]
            = $this->getSumTestEnv($NMITEM);
        $math->sum(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
        $events->wait();
        $bufferR->read($queue,$hostBufferR);
        $this->assertTrue(abs($hostBufferR[0]-$sum)<1e-7);
    }

    public function testSumInvalidBufferRObject()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$sum]
            = $this->getSumTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferR = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->sum(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testSumInvalidBufferXObject()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$sum]
            = $this->getSumTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferX = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->sum(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testSumInvalidQueueObject()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$sum]
            = $this->getSumTestEnv($NMITEM);
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->sum(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testSumInvalidEventsObject()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$sum]
            = $this->getSumTestEnv($NMITEM);
        // invalid Events object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->sum(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    //
    //  imax
    //

    protected function getimaxTestEnv($NMITEM)
    {
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $hostBufferX = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferR = $this->newHostBuffer(1,NDArray::int32);
        $max = 0.0;
        $imax = -1;
        for($i=0;$i<$NMITEM;$i++) {
            $hostBufferX[$i]=$i/$NMITEM;
            if(abs($hostBufferX[$i])>$max) {
                $max = abs($hostBufferX[$i]);
                $imax = $i;
            }
        }
        $bufferX = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferR = $ocl->Buffer($context,intval(32/8),
            OpenCL::CL_MEM_WRITE_ONLY);
        $math = $this->getMath();
        $events = $ocl->EventList();
        return [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$imax];
    }

    public function testimaxNormal()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$imax]
            = $this->getimaxTestEnv($NMITEM);
        $math->imax(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
        $events->wait();
        $bufferR->read($queue,$hostBufferR);
        $this->assertTrue(abs($hostBufferR[0]-$imax)<1e-7);
    }

    public function testimaxInvalidBufferRObject()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$imax]
            = $this->getimaxTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferR = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->imax(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testimaxInvalidBufferXObject()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$imax]
            = $this->getimaxTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferX = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->imax(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testimaxInvalidQueueObject()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$imax]
            = $this->getimaxTestEnv($NMITEM);
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->imax(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testimaxInvalidEventsObject()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$imax]
            = $this->getimaxTestEnv($NMITEM);
        // invalid Queue object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->imax(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    //
    //  imin
    //

    protected function getiminTestEnv($NMITEM)
    {
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $hostBufferX = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferR = $this->newHostBuffer(1,NDArray::int32);
        $min = 100000000;
        $imin = -1;
        for($i=0;$i<$NMITEM;$i++) {
            $hostBufferX[$i]=$i/$NMITEM;
            if(abs($hostBufferX[$i])<$min) {
                $min = abs($hostBufferX[$i]);
                $imin = $i;
            }
        }
        $bufferX = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferR = $ocl->Buffer($context,intval(32/8),
            OpenCL::CL_MEM_WRITE_ONLY);
        $math = $this->getMath();
        $events = $ocl->EventList();
        return [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$imin];
    }

    public function testiminNormal()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$imin]
            = $this->getiminTestEnv($NMITEM);
        $math->imin(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
        $events->wait();
        $bufferR->read($queue,$hostBufferR);
        $this->assertTrue(abs($hostBufferR[0]-$imin)<1e-7);
    }

    public function testiminInvalidBufferRObject()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$imin]
            = $this->getiminTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferR = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->imin(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testiminInvalidBufferXObject()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$imin]
            = $this->getiminTestEnv($NMITEM);
        // invalid Buffer object arguments
        $bufferX = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->imin(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testiminInvalidQueueObject()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$imin]
            = $this->getiminTestEnv($NMITEM);
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->imin(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    public function testiminInvalidEventsObject()
    {
        $NMITEM = 1024;
        [$queue,$math,$events,$bufferX,$bufferR,$hostBufferX,$hostBufferR,$imin]
            = $this->getiminTestEnv($NMITEM);
        // invalid Events object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->imin(
            $NMITEM,
            $bufferR,$offsetR=0,
            $bufferX,$offsetX=0,$incX=1,
            $queue,$events
        );
    }

    //
    //  hadamard
    //

    protected function gethadamardTestEnv($NMITEM)
    {
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $hostBufferX = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferY = $this->newHostBuffer($NMITEM,NDArray::float32);
        $hostBufferZ = $this->newHostBuffer($NMITEM,NDArray::float32);
        $testTruesR = $this->newHostBuffer($NMITEM,NDArray::float32);

        $alpha = 2.0;
        $beta = 3.0;
        for($i=0;$i<$NMITEM;$i++) {
            $hostBufferX[$i]=$i-$NMITEM/2;
            $hostBufferY[$i]=$NMITEM/2-$i;
            $hostBufferZ[$i]=$i;
            $testTruesR[$i]=$alpha*($i-$NMITEM/2)*($NMITEM/2-$i)+$beta*$i;
        }
        $bufferX = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferY = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferY);
        $bufferZ = $ocl->Buffer($context,intval($NMITEM*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferZ);

        $math = $this->getMath();
        $events = $ocl->EventList();
        return [
            $queue,$math,$events,$bufferX,$bufferY,$bufferZ,
            $hostBufferX,$hostBufferY,$hostBufferZ,$testTruesR,$alpha,$beta,
        ];
    }

    public function testHadamardNormal()
    {
        $NMITEM = 2048;
        [
            $queue,$math,$events,$bufferX,$bufferY,$bufferZ,
            $hostBufferX,$hostBufferY,$hostBufferZ,$testTruesR,$alpha,$beta,
        ] = $this->gethadamardTestEnv($NMITEM);
        $math->hadamard($NMITEM,$alpha,$bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,$beta,$bufferZ,$offsetZ=0,$incZ=1,$queue,$events);
        $events->wait();
        $bufferZ->read($queue,$hostBufferZ,intval($NMITEM*32/8));
        $equals = true;
        for($i=0;$i<$NMITEM;$i++) {
            if($testTruesR[$i]!=$hostBufferZ[$i]) {
                $equals = false;
                break;
            }
        }
        $this->assertTrue($equals);
    }

    public function testHadamardInvalidBufferXObject()
    {
        $NMITEM = 2048;
        [
            $queue,$math,$events,$bufferX,$bufferY,$bufferZ,
            $hostBufferX,$hostBufferY,$hostBufferZ,$testTruesR,$alpha,$beta,
        ] = $this->gethadamardTestEnv($NMITEM);
         // invalid buffer object arguments
         $bufferX = new \stdClass();
         $this->expectException(TypeError::class);
         //$this->expectExceptionMessage('??????????????????');
        $math->hadamard($NMITEM,$alpha,$bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,$beta,$bufferZ,$offsetZ=0,$incZ=1,$queue,$events);
    }

    public function testHadamardInvalidBufferYObject()
    {
        $NMITEM = 2048;
        [
            $queue,$math,$events,$bufferX,$bufferY,$bufferZ,
            $hostBufferX,$hostBufferY,$hostBufferZ,$testTruesR,$alpha,$beta,
        ] = $this->gethadamardTestEnv($NMITEM);
         // invalid buffer object arguments
         $bufferY = new \stdClass();
         $this->expectException(TypeError::class);
         //$this->expectExceptionMessage('??????????????????');
        $math->hadamard($NMITEM,$alpha,$bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,$beta,$bufferZ,$offsetZ=0,$incZ=1,$queue,$events);
    }

    public function testHadamardInvalidBufferZObject()
    {
        $NMITEM = 2048;
        [
            $queue,$math,$events,$bufferX,$bufferY,$bufferZ,
            $hostBufferX,$hostBufferY,$hostBufferZ,$testTruesR,$alpha,$beta,
        ] = $this->gethadamardTestEnv($NMITEM);
         // invalid buffer object arguments
         $bufferZ = new \stdClass();
         $this->expectException(TypeError::class);
         //$this->expectExceptionMessage('??????????????????');
        $math->hadamard($NMITEM,$alpha,$bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,$beta,$bufferZ,$offsetZ=0,$incZ=1,$queue,$events);
    }

    public function testHadamardInvalidQueueObject()
    {
        $NMITEM = 2048;
        [
            $queue,$math,$events,$bufferX,$bufferY,$bufferZ,
            $hostBufferX,$hostBufferY,$hostBufferZ,$testTruesR,$alpha,$beta,
        ] = $this->gethadamardTestEnv($NMITEM);
         // invalid Queue object arguments
         $queue = new \stdClass();
         $this->expectException(TypeError::class);
         //$this->expectExceptionMessage('??????????????????');
        $math->hadamard($NMITEM,$alpha,$bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,$beta,$bufferZ,$offsetZ=0,$incZ=1,$queue,$events);
    }

    public function testHadamardInvalidEventsObject()
    {
        $NMITEM = 2048;
        [
            $queue,$math,$events,$bufferX,$bufferY,$bufferZ,
            $hostBufferX,$hostBufferY,$hostBufferZ,$testTruesR,$alpha,$beta,
        ] = $this->gethadamardTestEnv($NMITEM);
         // invalid Events object arguments
         $events = new \stdClass();
         $this->expectException(TypeError::class);
         //$this->expectExceptionMessage('??????????????????');
        $math->hadamard($NMITEM,$alpha,$bufferX,$offsetX=0,$incX=1,
            $bufferY,$offsetY=0,$incY=1,$beta,$bufferZ,$offsetZ=0,$incZ=1,$queue,$events);
    }

    //
    //  im2col
    //

    protected function getim2colTestEnv(
        $kernel_mode,
        $channels,
        $height,
        $width,
        $kernel_h,
        $kernel_w,
        $pad_h,
        $pad_w,
        $stride_h,
        $stride_w,
        $dilation_h,
        $dilation_w,
        $im_offset,
        $col_offset
        )
    {
        $out_h = $height-$kernel_h+1;
        $out_w = $width-$kernel_w+1;
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $host_im_buffer  = $this->newHostBuffer($height*$width*$channels,NDArray::float32);
        #$host_col_buffer = $this->newHostBuffer(($height-$kernel_h+1)*($width-$kernel_w+1)*4*$channels,NDArray::float32);
        $host_col_buffer = $this->newHostBuffer($out_h*$out_w*4*$channels,NDArray::float32);
        for($i=0;$i<count($host_im_buffer);$i++) {
            $host_im_buffer[$i]=$i;
        }
        for($i=0;$i<count($host_col_buffer);$i++) {
            $host_col_buffer[$i]=100000;
        }
        $trues = [];
        $i = 0;
        for($ch_id=0;$ch_id<$channels;$ch_id++) {
            for($kh_id=0;$kh_id<$kernel_h;$kh_id++) {
                for($kw_id=0;$kw_id<$kernel_w;$kw_id++) {
                    for($fh_id=0;$fh_id<$out_h;$fh_id++) {
                        for($fw_id=0;$fw_id<$out_w;$fw_id++) {
                            $image_index =
                                $ch_id*($height*$width)+
                                ($kernel_h-$kh_id-1)*($width)+ ###### CAUTION #####
                                ($kernel_w-$kw_id-1)*(1)+      ###### CAUTION #####
                                $fh_id*($width)+
                                $fw_id*(1);
                            $trues[$i] = $host_im_buffer[$image_index];
                            $i++;
                        }
                    }
                }
            }
        }
        $im_buffer = $ocl->Buffer($context,intval(count($host_im_buffer)*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $host_im_buffer);
        $col_buffer = $ocl->Buffer($context,intval(count($host_col_buffer)*32/8),
            OpenCL::CL_MEM_WRITE_ONLY,null,0,NDArray::float32);
        $math = $this->getMath();
        $events = $ocl->EventList();
        return [
            $queue,$math,$events,$im_buffer,$col_buffer,
            $host_im_buffer,$host_col_buffer,$trues,
        ];
    }

    public function testim2colNormal()
    {
        $kernel_mode = Math::CONVOLUTION;
        $channels=3;
        $height=4;
        $width=4;
        $kernel_h = 2;
        $kernel_w = 2;
        $pad_h = 0;
        $pad_w = 0;
        $stride_h = 1;
        $stride_w = 1;
        $dilation_h=1;
        $dilation_w=1;
        $im_offset =0;
        $col_offset =0;
        [
            $queue,$math,$events,$im_buffer,$col_buffer,
            $host_im_buffer,$host_col_buffer,$trues,
        ] = $this->getim2colTestEnv(
            $kernel_mode,
            $channels,
            $height,
            $width,
            $kernel_h,
            $kernel_w,
            $pad_h,
            $pad_w,
            $stride_h,
            $stride_w,
            $dilation_h,
            $dilation_w,
            $im_offset,
            $col_offset
        );
        $math->im2col($kernel_mode,
            $channels,$height,$width,
            $kernel_h,$kernel_w,
            $pad_h,$pad_w,
            $stride_h,$stride_w,
            $dilation_h,$dilation_w,
            $im_buffer,$im_offset,
            $col_buffer,$col_offset,
            $queue,$events
        );
        $events->wait();
        $col_buffer->read($queue,$host_col_buffer);
        $equals = true;
        for($i=0;$i<count($host_col_buffer);$i++) {
            //echo $host_col_buffer[$i]."=".$trues[$i].",";
            if($trues[$i]!=$host_col_buffer[$i]) {
                $equals = false;
                break;
            }
        }
        $this->assertTrue($equals);
    }

    public function testim2colInvalidimbuffer()
    {
        $kernel_mode = Math::CONVOLUTION;
        $channels=3;
        $height=4;
        $width=4;
        $kernel_h = 2;
        $kernel_w = 2;
        $pad_h = 0;
        $pad_w = 0;
        $stride_h = 1;
        $stride_w = 1;
        $dilation_h=1;
        $dilation_w=1;
        $im_offset =0;
        $col_offset =0;
        [
            $queue,$math,$events,$im_buffer,$col_buffer,
            $host_im_buffer,$host_col_buffer,$trues,
        ] = $this->getim2colTestEnv(
            $kernel_mode,
            $channels,
            $height,
            $width,
            $kernel_h,
            $kernel_w,
            $pad_h,
            $pad_w,
            $stride_h,
            $stride_w,
            $dilation_h,
            $dilation_w,
            $im_offset,
            $col_offset
        );
        // invalid buffer object arguments
        $im_buffer = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->im2col($kernel_mode,
            $channels,$height,$width,
            $kernel_h,$kernel_w,
            $pad_h,$pad_w,
            $stride_h,$stride_w,
            $dilation_h,$dilation_w,
            $im_buffer,$im_offset,
            $col_buffer,$col_offset,
            $queue,$events
        );
    }

    public function testim2colInvalidcolbuffer()
    {
        $kernel_mode = Math::CONVOLUTION;
        $channels=3;
        $height=4;
        $width=4;
        $kernel_h = 2;
        $kernel_w = 2;
        $pad_h = 0;
        $pad_w = 0;
        $stride_h = 1;
        $stride_w = 1;
        $dilation_h=1;
        $dilation_w=1;
        $im_offset =0;
        $col_offset =0;
        [
            $queue,$math,$events,$im_buffer,$col_buffer,
            $host_im_buffer,$host_col_buffer,$trues,
        ] = $this->getim2colTestEnv(
            $kernel_mode,
            $channels,
            $height,
            $width,
            $kernel_h,
            $kernel_w,
            $pad_h,
            $pad_w,
            $stride_h,
            $stride_w,
            $dilation_h,
            $dilation_w,
            $im_offset,
            $col_offset
        );
        // invalid buffer object arguments
        $col_buffer = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->im2col($kernel_mode,
            $channels,$height,$width,
            $kernel_h,$kernel_w,
            $pad_h,$pad_w,
            $stride_h,$stride_w,
            $dilation_h,$dilation_w,
            $im_buffer,$im_offset,
            $col_buffer,$col_offset,
            $queue,$events
        );
    }

    public function testim2colInvalidQueueObject()
    {
        $kernel_mode = Math::CONVOLUTION;
        $channels=3;
        $height=4;
        $width=4;
        $kernel_h = 2;
        $kernel_w = 2;
        $pad_h = 0;
        $pad_w = 0;
        $stride_h = 1;
        $stride_w = 1;
        $dilation_h=1;
        $dilation_w=1;
        $im_offset =0;
        $col_offset =0;
        [
            $queue,$math,$events,$im_buffer,$col_buffer,
            $host_im_buffer,$host_col_buffer,$trues,
        ] = $this->getim2colTestEnv(
            $kernel_mode,
            $channels,
            $height,
            $width,
            $kernel_h,
            $kernel_w,
            $pad_h,
            $pad_w,
            $stride_h,
            $stride_w,
            $dilation_h,
            $dilation_w,
            $im_offset,
            $col_offset
        );
        // invalid Queue object arguments
        $queue = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->im2col($kernel_mode,
            $channels,$height,$width,
            $kernel_h,$kernel_w,
            $pad_h,$pad_w,
            $stride_h,$stride_w,
            $dilation_h,$dilation_w,
            $im_buffer,$im_offset,
            $col_buffer,$col_offset,
            $queue,$events
        );
    }

    public function testim2colInvalidEventsObject()
    {
        $kernel_mode = Math::CONVOLUTION;
        $channels=3;
        $height=4;
        $width=4;
        $kernel_h = 2;
        $kernel_w = 2;
        $pad_h = 0;
        $pad_w = 0;
        $stride_h = 1;
        $stride_w = 1;
        $dilation_h=1;
        $dilation_w=1;
        $im_offset =0;
        $col_offset =0;
        [
            $queue,$math,$events,$im_buffer,$col_buffer,
            $host_im_buffer,$host_col_buffer,$trues,
        ] = $this->getim2colTestEnv(
            $kernel_mode,
            $channels,
            $height,
            $width,
            $kernel_h,
            $kernel_w,
            $pad_h,
            $pad_w,
            $stride_h,
            $stride_w,
            $dilation_h,
            $dilation_w,
            $im_offset,
            $col_offset
        );
        // invalid Events object arguments
        $events = new \stdClass();
        $this->expectException(TypeError::class);
        //$this->expectExceptionMessage('??????????????????');
        $math->im2col($kernel_mode,
            $channels,$height,$width,
            $kernel_h,$kernel_w,
            $pad_h,$pad_w,
            $stride_h,$stride_w,
            $dilation_h,$dilation_w,
            $im_buffer,$im_offset,
            $col_buffer,$col_offset,
            $queue,$events
        );
    }

    //
    //  col2im
    //

    protected function getcol2imTestEnv(
        $kernel_mode,
        $channels,
        $height,
        $width,
        $kernel_h,
        $kernel_w,
        $pad_h,
        $pad_w,
        $stride_h,
        $stride_w,
        $dilation_h,
        $dilation_w,
        $im_offset,
        $col_offset
        )
    {
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $host_col_buffer = $this->newHostBuffer(($height-$kernel_h+1)*($width-$kernel_w+1)*4*$channels,NDArray::float32);
        $host_im_buffer  = $this->newHostBuffer($height*$width*$channels,NDArray::float32);
        $filter_h = $height-$kernel_h+1;
        $filter_w = $width-$kernel_w+1;
        $trues = [];
        for($i=0;$i<count($host_im_buffer);$i++) {
            $host_im_buffer[$i]=100000;
            $trues[$i]=100000;
        }
        $i = 0;
        for($ch_id=0;$ch_id<$channels;$ch_id++) {
            for($kh_id=$kernel_h-1;$kh_id>=0;$kh_id--) {
                for($kw_id=$kernel_w-1;$kw_id>=0;$kw_id--) {
                    for($fh_id=0;$fh_id<$filter_h;$fh_id++) {
                        for($fw_id=0;$fw_id<$filter_w;$fw_id++) {
                            $idx = $ch_id*($height*$width)+
                                $kh_id*($width)+
                                $kw_id*(1)+
                                $fh_id*($width)+
                                $fw_id*(1);
                            $host_col_buffer[$i] = $idx;
                            $i++;
                            $trues[$idx] += $idx;
                        }
                    }
                }
            }
        }
        $col_buffer = $ocl->Buffer($context,intval(count($host_col_buffer)*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,$host_col_buffer);
        $im_buffer = $ocl->Buffer($context,intval(count($host_im_buffer)*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,$host_im_buffer,0,NDArray::float32);
        $math = $this->getMath();
        $events = $ocl->EventList();
        return [
            $queue,$math,$events,$im_buffer,$col_buffer,
            $host_im_buffer,$host_col_buffer,$trues,
        ];
    }

    public function testcol2imNormal()
    {
        $kernel_mode = Math::CONVOLUTION;
        $channels=3;
        $height=4;
        $width=4;
        $kernel_h = 2;
        $kernel_w = 2;
        $pad_h = 0;
        $pad_w = 0;
        $stride_h = 1;
        $stride_w = 1;
        $dilation_h=1;
        $dilation_w=1;
        $im_offset =0;
        $col_offset =0;
        [
            $queue,$math,$events,$im_buffer,$col_buffer,
            $host_im_buffer,$host_col_buffer,$trues,
        ] = $this->getcol2imTestEnv(
            $kernel_mode,
            $channels,
            $height,
            $width,
            $kernel_h,
            $kernel_w,
            $pad_h,
            $pad_w,
            $stride_h,
            $stride_w,
            $dilation_h,
            $dilation_w,
            $im_offset,
            $col_offset
        );
        $math->col2im($kernel_mode,
            $channels,$height,$width,
            $kernel_h,$kernel_w,
            $pad_h,$pad_w,
            $stride_h,$stride_w,
            $dilation_h,$dilation_w,
            $col_buffer,$col_offset,
            $im_buffer,$im_offset,
            $queue,$events);
        $events->wait();
        $im_buffer->read($queue,$host_im_buffer);
        $equals = true;
        for($i=0;$i<count($host_im_buffer);$i++) {
            if($trues[$i]!=$host_im_buffer[$i]) {
                $equals = false;
                break;
            }
        }
        $this->assertTrue($equals);
    }

    //
    //  convgemm
    //

    protected function getconvgemmTestEnv(
        $kernel_mode,
        $channels,
        $height,
        $width,
        $kernel_h,
        $kernel_w,
        $pad_h,
        $pad_w,
        $stride_h,
        $stride_w,
        $dilation_h,
        $dilation_w,
        $num_kernels,
        $batch_count,
        $im_offset,
        $kernel_offset,
        $result_offset
    )
    {
        $out_h = $height-$kernel_h+1;
        $out_w = $width-$kernel_w+1;
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $host_im_buffer  = $this->newHostBuffer($height*$width*$channels*$batch_count,NDArray::float32);
        $host_kernel_buffer = $this->newHostBuffer($kernel_w*$kernel_w*$channels*$num_kernels,NDArray::float32);
        $host_result_buffer = $this->newHostBuffer($out_h*$out_w*$num_kernels*$batch_count,NDArray::float32);
        for($i=0;$i<count($host_im_buffer);$i++) {
            $host_im_buffer[$i]=$i;
        }
        for($i=0;$i<count($host_kernel_buffer);$i++) {
            $host_kernel_buffer[$i]=1;
        }
        $trues = [];
        for($i=0;$i<count($host_result_buffer);$i++) {
            $host_result_buffer[$i]=100000;
            $trues[$i]=0;
        }
        for($b_id=0;$b_id<$batch_count;$b_id++) {
            for($ko_id=0;$ko_id<$num_kernels;$ko_id++) {
                for($ch_id=0;$ch_id<$channels;$ch_id++) {
                    for($kh_id=0;$kh_id<$kernel_h;$kh_id++) {
                        for($kw_id=0;$kw_id<$kernel_w;$kw_id++) {
                            for($fh_id=0;$fh_id<$out_h;$fh_id++) {
                                for($fw_id=0;$fw_id<$out_w;$fw_id++) {
                                    $idx =
                                        $b_id*$num_kernels*$out_h*$out_w+
                                        $ko_id*$out_h*$out_w+
                                        $fh_id*$out_w+
                                        $fw_id;
                                    $image_index =
                                        $b_id*($height*$width*$channels)+
                                        $ch_id*($height*$width)+
                                        ($kernel_h-$kh_id-1)*($width)+ ###### CAUTION #####
                                        ($kernel_w-$kw_id-1)*(1)+      ###### CAUTION #####
                                        #$kh_id*($width)+
                                        #$kw_id*(1)+
                                        $fh_id*($width)+
                                        $fw_id*(1);
                                    $kernel_index =
                                        $ko_id*$channels*$kernel_h*$kernel_w+
                                        $ch_id*$kernel_h*$kernel_w+
                                        $kh_id*$kernel_w+
                                        $kw_id
                                        ;
                                    $trues[$idx] +=
                                        $host_im_buffer[$image_index]*
                                        $host_kernel_buffer[$kernel_index];
                                }
                            }
                        }
                    }
                }
            }
        }
        $im_buffer = $ocl->Buffer($context,intval(count($host_im_buffer)*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $host_im_buffer);
        $kernel_buffer = $ocl->Buffer($context,intval(count($host_kernel_buffer)*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $host_kernel_buffer);
        $result_buffer = $ocl->Buffer($context,intval(count($host_result_buffer)*32/8),
            OpenCL::CL_MEM_WRITE_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $host_result_buffer);
        $math = $this->getMath();
        $events = $ocl->EventList();
        return [
            $queue,$math,$events,$im_buffer,$kernel_buffer,$result_buffer,
            $host_im_buffer,$host_kernel_buffer,$host_result_buffer,$trues,
        ];
    }

    public function testconvgemmNormal()
    {
        $kernel_mode = Math::CONVOLUTION;
        $channels=3;
        $height=4;
        $width=4;
        $kernel_h = 3;
        $kernel_w = 3;
        $pad_h = 0;
        $pad_w = 0;
        $stride_h = 1;
        $stride_w = 1;
        $dilation_h=1;
        $dilation_w=1;
        $num_kernels=5;
        $batch_count=6;
        $im_offset=0;
        $kernel_offset=0;
        $result_offset=0;
        [
            $queue,$math,$events,$im_buffer,$kernel_buffer,$result_buffer,
            $host_im_buffer,$host_kernel_buffer,$host_result_buffer,$trues,
        ] = $this->getconvgemmTestEnv(
            $kernel_mode,
            $channels,
            $height,
            $width,
            $kernel_h,
            $kernel_w,
            $pad_h,
            $pad_w,
            $stride_h,
            $stride_w,
            $dilation_h,
            $dilation_w,
            $num_kernels,
            $batch_count,
            $im_offset,
            $kernel_offset,
            $result_offset
        );
        $math->convgemm($kernel_mode,
            $channels,$height,$width,
            $kernel_h,$kernel_w,
            $pad_h,$pad_w,
            $stride_h,$stride_w,
            $dilation_h,$dilation_w,
            $num_kernels,$batch_count,
            $im_buffer,$im_offset,
            $kernel_buffer,$kernel_offset,
            $result_buffer,$result_offset,
            $queue,$events);
        $events->wait();
        $result_buffer->read($queue,$host_result_buffer);
        $equals = true;
        for($i=0;$i<count($host_result_buffer);$i++) {
            #echo $host_result_buffer[$i].",";
            if($trues[$i]!=$host_result_buffer[$i]) {
                $equals = false;
                break;
            }
        }
        $this->assertTrue($equals);
    }

    //
    //  axpyBatched
    //

    protected function getaxpyBatchedTestEnv(
        $NMITEM,
        $batch_count
    )
    {
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $hostBufferX = $this->newHostBuffer($NMITEM*$batch_count,NDArray::float32);
        $hostBufferY = $this->newHostBuffer($NMITEM*$batch_count,NDArray::float32);
        $hostAlphas  = $this->newHostBuffer($batch_count,NDArray::float32);
        $hostOffsetsX = $this->newHostBuffer($batch_count,NDArray::int64);
        $hostOffsetsY = $this->newHostBuffer($batch_count,NDArray::int64);
        for($i=0;$i<$batch_count;$i++) {
            $hostAlphas[$i]=$batch_count-$i;
            $hostOffsetsX[$i]=($batch_count-$i-1)*$NMITEM;
            $hostOffsetsY[$i]=$i*$NMITEM;
        }
        for($i=0;$i<$batch_count;$i++) {
            for($j=0;$j<$NMITEM;$j++) {
                $hostBufferX[$i*$NMITEM+$j]=($i+1)*1000;#$j+$i*1000;
                $hostBufferY[$i*$NMITEM+$j]=($i+1)*100000;#$j+$i*100000;
            }
        }
        $bufferX = $ocl->Buffer($context,intval(count($hostBufferX)*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferX);
        $bufferY = $ocl->Buffer($context,intval(count($hostBufferY)*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferY);
        $math = $this->getMath();
        $events = $ocl->EventList();
        return [
            $queue,$math,$events,$bufferX,$bufferY,
            $hostBufferX,$hostBufferY,$hostAlphas,$hostOffsetsX,$hostOffsetsY,
        ];
    }

    public function testaxpyBatchedNormal()
    {
        $NMITEM = 3;
        $batch_count = 4;
        [
            $queue,$math,$events,$bufferX,$bufferY,
            $hostBufferX,$hostBufferY,$hostAlphas,$hostOffsetsX,$hostOffsetsY,
        ] = $this->getaxpyBatchedTestEnv(
            $NMITEM,
            $batch_count
        );
        $math->axpyBatched(
            $NMITEM,
            $hostAlphas,$offsetA=0,
            $bufferX,$hostOffsetsX,$offsetX=0,$incX=1,
            $bufferY,$hostOffsetsY,$offsetY=0,$incY=1,
            $batch_count,
            $queue,$events
        );
        $events->wait();
        $bufferY->read($queue,$hostBufferY);
        #echo "R:\n";
        $equals = true;
        for($i=0;$i<$batch_count;$i++) {
            for($j=0;$j<$NMITEM;$j++) {
                #echo $hostBufferY[$i*$NMITEM+$j],",";
                if($hostBufferY[$i*$NMITEM+$j]!=(pow($batch_count-$i,2)*1000)+($i+1)*100000) {
                    $equals = false;
                    break;
                }
            }
            #echo "\n";
            if($equals==false) {
                break;
            }
        }
        $this->assertTrue($equals);
    }

    /*

    //
    //  gemmBatched
    //

    protected function getgemmBatchedTestEnv(
        $batch_count,
        $m,
        $n,
        $k,
    )
    {
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $hostAlpha   = $this->newHostBuffer($batch_count,NDArray::float32);
        $hostBeta    = $this->newHostBuffer($batch_count,NDArray::float32);
        $hostBufferA = $this->newHostBuffer($batch_count*$m*$k,NDArray::float32);
        $hostBufferB = $this->newHostBuffer($batch_count*$k*$n,NDArray::float32);
        $hostBufferC = $this->newHostBuffer($batch_count*$m*$n,NDArray::float32);
        $testTruesR  = $this->newHostBuffer($batch_count*$m*$n,NDArray::float32);
        $offsetsA = $this->newHostBuffer($batch_count,NDArray::int64);
        $offsetsB = $this->newHostBuffer($batch_count,NDArray::int64);
        $offsetsC = $this->newHostBuffer($batch_count,NDArray::int64);
        
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
        
        $openmath = $this->getMatlib();
        $openmath->randomSequence($batch_count,$batch_count,$offsetsA, 0, 1,random_int(~PHP_INT_MAX,PHP_INT_MAX));
        $openmath->randomSequence($batch_count,$batch_count,$offsetsB, 0, 1,random_int(~PHP_INT_MAX,PHP_INT_MAX));
        $openmath->randomSequence($batch_count,$batch_count,$offsetsC, 0, 1,random_int(~PHP_INT_MAX,PHP_INT_MAX));
        #echo "offsetsA=";
        for($ii=0;$ii<$batch_count;$ii++) {
            #echo $offsetsA[$ii] .",";
            $offsetsA[$ii] = $offsetsA[$ii]*$m*$k;
        }
        #echo "\n";
        #echo "offsetsB=";
        for($ii=0;$ii<$batch_count;$ii++) {
            #echo $offsetsB[$ii].",";
            $offsetsB[$ii] = $offsetsB[$ii]*$k*$n;
        }
        #echo "\n";
        #echo "offsetsC=";
        for($ii=0;$ii<$batch_count;$ii++) {
            #echo $offsetsC[$ii].",";
            $offsetsC[$ii] = $offsetsC[$ii]*$m*$n;
        }
        #echo "\n";
        $openblas = $this->getOpenBLAS();
        for($ii=0;$ii<$batch_count;$ii++) {
            $openblas->gemm(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
                $hostAlpha[$ii],
                $hostBufferA, $offsetsA[$ii], $k,
                $hostBufferB, $offsetsB[$ii], $n,
                $hostBeta[$ii],
                $testTruesR,  $offsetsC[$ii], $n
            );
        }
        
        $bufferA = $ocl->Buffer($context,intval(count($hostBufferA)*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferA);
        $bufferB = $ocl->Buffer($context,intval(count($hostBufferB)*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferB);
        $bufferC = $ocl->Buffer($context,intval(count($hostBufferC)*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferC);
        $math = $this->getMath();
        $events = $ocl->EventList();
        return [
            $queue,$math,$events,$bufferA,$bufferB,$bufferC,
            $offsetsA,$offsetsB,$offsetsC,
            $hostBufferC,$hostAlpha,$hostBeta,$testTruesR,
        ];
    }

    public function testgemmBatchedFullRange()
    {
        $batch_count = 4;
        $m = 8;
        $n = 6;
        $k = 4;
        [
            $queue,$math,$events,$bufferA,$bufferB,$bufferC,
            $offsetsA,$offsetsB,$offsetsC,
            $hostBufferC,$hostAlpha,$hostBeta,$testTruesR,
        ] = $this->getgemmBatchedTestEnv(
            $batch_count,
            $m,
            $n,
            $k,
        );
        $math->gemmBatched(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
            $hostAlpha,0,
            $bufferA,$offsetsA,$offsetA=0,$ldA=$k,
            $bufferB,$offsetsB,$offsetB=0,$ldA=$n,
            $hostBeta,0,
            $bufferC,$offsetsC,$offsetC=0,$ldC=$n,
            $batch_count,
            $queue,$events,
        );
        $events->wait();
        $bufferC->read($queue,$hostBufferC);
        $equals = true;
        for($ii=0;$ii<$batch_count;$ii++) {
            for($i=0;$i<$m*$n;$i++) {
                #echo $hostBufferC[$ii*$m*$n+$i].",";
                if($hostBufferC[$ii*$m*$n+$i]!=$testTruesR[$ii*$m*$n+$i]) {
                    $equals = false;
                    break;
                }
            }
            #echo "\n";
            if($equals==false) {
                break;
            }
        }
        $this->assertTrue($equals);
    }
    */

    //
    //  gemmStridedBatched
    //
    protected function translate_gemmStridedBatched(
        NDArray $A,
        NDArray $B,
        $alpha,
        $beta,
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
        if($A->ndim()!=3) {
            throw new InvalidArgumentException('"A" must 3D matrix');
        }
        if($B->ndim()!=3) {
            throw new InvalidArgumentException('"B" must 3D matrix');
        }
        if($A->dtype()!=$B->dtype()) {
            throw new InvalidArgumentException('Unmatch data type "A" and "B"');
        }

        $shapeA = $A->shape();
        $batchA = array_shift($shapeA);
        $shapeB = $B->shape();
        $batchB = array_shift($shapeB);
        if($batchA!=$batchB) {
            throw new InvalidArgumentException('Unmatch batch size of "A" and "B"');
        }
        if($transA) {
            $shapeA = [$shapeA[1],$shapeA[0]];
        }
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
            if($this->isComplex($A->dtype())) {
                $alpha = C(1.0);
            } else {
                $alpha = 1.0;
            }
        }
        if($beta===null) {
            if($this->isComplex($A->dtype())) {
                $beta = C(0.0);
            } else {
                $beta = 0.0;
            }
        }
        if($C!=null) {
            $shapeC = $C->shape();
            $batchC = array_shift($shapeC);
            if($batchA!=$batchC) {
                throw new InvalidArgumentException('Unmatch batch size of "A" and "C"');
            }
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
        $strideA = $M*$K;
        $strideB = $K*$N;
        $strideC = $M*$N;
        $batchCount = $batchA;

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
            $AA,$offA,$lda,$strideA,
            $BB,$offB,$ldb,$strideB,
            $beta,
            $CC,$offC,$ldc,$strideC,
            $batchCount,
            $this->queue,$events,
        ];
    }

    public function testGemmStridedBatchedNormal()
    {
        $blas = $this->getMath();

        // float32
        $dtype = NDArray::float32;
        $A = $this->array([
            [[1,2,3],[4,5,6]],
            [[2,3,4],[5,6,7]],
        ],$dtype);
        $B = $this->array([
            [[1,0,0],[0,1,0],[0,0,1]],
            [[2,0,0],[0,2,0],[0,0,2]],
        ],$dtype);
        $alpha = 1.0;
        $beta  = 0.0;
        $C = $this->ones([2,2,3],$dtype);
        $transA = false;
        $transB = false;

        [
            $order,$transA,$transB,
            $M,$N,$K,
            $alpha,
            $AA,$offA,$lda,$strideA,
            $BB,$offB,$ldb,$strideB,
            $beta,
            $CC,$offC,$ldc,$strideC,
            $batchCount,
            $queue,$events,
        ] = $this->translate_gemmStridedBatched($A,$B,
                $alpha, $beta, $C, $transA, $transB
        );

        $blas->gemmStridedBatched(
            $order,$transA,$transB,
            $M,$N,$K,
            $alpha,
            $AA,$offA,$lda,$strideA,
            $BB,$offB,$ldb,$strideB,
            $beta,
            $CC,$offC,$ldc,$strideC,
            $batchCount,
            $queue,$events
        );
        $events->wait();

        $this->assertEquals([
            [[1,2,3],[ 4, 5, 6]],
            [[4,6,8],[10,12,14]],
        ],$C->toArray());

        /*
        // complex64
        $dtype = NDArray::complex64;
        $A = $this->array($this->toComplex([
            [[1,2,3],[4,5,6]],
            [[2,3,4],[5,6,7]],
        ]),$dtype);
        $B = $this->array($this->toComplex([
            [[1,0,0],[0,1,0],[0,0,1]],
            [[2,0,0],[0,2,0],[0,0,2]],
        ]),$dtype);
        $alpha = C(1.0);
        $beta  = C(0.0);
        $C = $this->ones([2,2,3],$dtype);
        $transA = false;
        $transB = false;

        [
            $order,$transA,$transB,
            $M,$N,$K,
            $alpha,
            $AA,$offA,$lda,$strideA,
            $BB,$offB,$ldb,$strideB,
            $beta,
            $CC,$offC,$ldc,$strideC,
            $batchCount,
            $queue,$events,
        ] = $this->translate_gemmStridedBatched($A,$B,
                $alpha, $beta, $C, $transA, $transB
        );

        $blas->gemmStridedBatched(
            $order,$transA,$transB,
            $M,$N,$K,
            $alpha,
            $AA,$offA,$lda,$strideA,
            $BB,$offB,$ldb,$strideB,
            $beta,
            $CC,$offC,$ldc,$strideC,
            $batchCount,
            $queue,$events,
        );
        $events->wait();

        $this->assertEquals($this->toComplex([
            [[1,2,3],[ 4, 5, 6]],
            [[4,6,8],[10,12,14]],
        ]),$C->toArray());
        */

        if($this->fp64()) {
            // float64
            $dtype = NDArray::float64;
            $A = $this->array([
                [[1,2,3],[4,5,6]],
                [[2,3,4],[5,6,7]],
            ],$dtype);
            $B = $this->array([
                [[1,0,0],[0,1,0],[0,0,1]],
                [[2,0,0],[0,2,0],[0,0,2]],
            ],$dtype);
            $alpha = 1.0;
            $beta  = 0.0;
            $C = $this->ones([2,2,3],$dtype);
            $transA = false;
            $transB = false;

            [
                $order,$transA,$transB,
                $M,$N,$K,
                $alpha,
                $AA,$offA,$lda,$strideA,
                $BB,$offB,$ldb,$strideB,
                $beta,
                $CC,$offC,$ldc,$strideC,
                $batchCount,
                $queue,$events,
            ] = $this->translate_gemmStridedBatched($A,$B,
                    $alpha, $beta, $C, $transA, $transB
            );

            $blas->gemmStridedBatched(
                $order,$transA,$transB,
                $M,$N,$K,
                $alpha,
                $AA,$offA,$lda,$strideA,
                $BB,$offB,$ldb,$strideB,
                $beta,
                $CC,$offC,$ldc,$strideC,
                $batchCount,
                $queue,$events
            );
            $events->wait();

            $this->assertEquals([
                [[1,2,3],[ 4, 5, 6]],
                [[4,6,8],[10,12,14]],
            ],$C->toArray());

            /*
            // complex128
            $dtype = NDArray::complex128;
            $A = $this->array($this->toComplex([
                [[1,2,3],[4,5,6]],
                [[2,3,4],[5,6,7]],
            ]),$dtype);
            $B = $this->array($this->toComplex([
                [[1,0,0],[0,1,0],[0,0,1]],
                [[2,0,0],[0,2,0],[0,0,2]],
            ]),$dtype);
            $alpha = C(1.0);
            $beta  = C(0.0);
            $C = $this->ones([2,2,3],$dtype);
            $transA = false;
            $transB = false;

            [
                $order,$transA,$transB,
                $M,$N,$K,
                $alpha,
                $AA,$offA,$lda,$strideA,
                $BB,$offB,$ldb,$strideB,
                $beta,
                $CC,$offC,$ldc,$strideC,
                $batchCount,
                $queue,$events,
            ] = $this->translate_gemmStridedBatched($A,$B,
                    $alpha, $beta, $C, $transA, $transB
            );

            $blas->gemmStridedBatched(
                $order,$transA,$transB,
                $M,$N,$K,
                $alpha,
                $AA,$offA,$lda,$strideA,
                $BB,$offB,$ldb,$strideB,
                $beta,
                $CC,$offC,$ldc,$strideC,
                $batchCount,
                $queue,$events,
            );
            $events->wait();

            $this->assertEquals($this->toComplex([
                [[1,2,3],[ 4, 5, 6]],
                [[4,6,8],[10,12,14]],
            ]),$C->toArray());
            */

        }
    }

    protected function getgemmStridedBatchedTestEnv(
        $batch_count,
        $m,
        $n,
        $k
    )
    {
        $strideA = $m*$k;
        $strideB = $k*$n;
        $strideC = $m*$n;
        $ocl = $this->getOpenCL();
        $context = $this->newContextFromType($ocl);
        $queue = $ocl->CommandQueue($context);
        $hostBufferA = $this->newHostBuffer($batch_count*$m*$k,NDArray::float32);
        $hostBufferB = $this->newHostBuffer($batch_count*$k*$n,NDArray::float32);
        $hostBufferC = $this->newHostBuffer($batch_count*$m*$n,NDArray::float32);
        $testTruesR  = $this->newHostBuffer($batch_count*$m*$n,NDArray::float32);
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
                
        $openblas = $this->getOpenBLAS();
        for($ii=0;$ii<$batch_count;$ii++) {
            $openblas->gemm(
                BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,
                $m,$n,$k,
                $alpha,
                $hostBufferA, $strideA*$ii, $k,
                $hostBufferB, $strideB*$ii, $n,
                $beta,
                $testTruesR,  $strideC*$ii, $n
            );
        }
        
        $bufferA = $ocl->Buffer($context,intval(count($hostBufferA)*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferA);
        $bufferB = $ocl->Buffer($context,intval(count($hostBufferB)*32/8),
            OpenCL::CL_MEM_READ_ONLY|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferB);
        $bufferC = $ocl->Buffer($context,intval(count($hostBufferC)*32/8),
            OpenCL::CL_MEM_READ_WRITE|OpenCL::CL_MEM_COPY_HOST_PTR,
            $hostBufferC);
        $math = $this->getMath();
        $events = $ocl->EventList();
        return [
            $queue,$math,$events,$bufferA,$bufferB,$bufferC,
            $hostBufferC,$alpha,$beta,$strideA,$strideB,$strideC,$testTruesR,
        ];
    }

    public function testgemmStridedBatchedFullRange()
    {
        $batch_count = 4;
        $m = 8;
        $n = 6;
        $k = 4;
        [
            $queue,$math,$events,$bufferA,$bufferB,$bufferC,
            $hostBufferC,$alpha,$beta,$strideA,$strideB,$strideC,$testTruesR,
        ] = $this->getgemmStridedBatchedTestEnv(
            $batch_count,
            $m,
            $n,
            $k
        );
        $math->gemmStridedBatched(BLAS::RowMajor,BLAS::NoTrans,BLAS::NoTrans,$m,$n,$k,
            $alpha,
            $bufferA,$offsetA=0,$ldA=$k,$strideA,
            $bufferB,$offsetB=0,$ldA=$n,$strideB,
            $beta,
            $bufferC,$offsetC=0,$ldC=$n,$strideC,
            $batch_count,
            $queue,$events
        );
        $events->wait();
        $bufferC->read($queue,$hostBufferC);
        $equals = true;
        for($ii=0;$ii<$batch_count;$ii++) {
            for($i=0;$i<$m*$n;$i++) {
                #echo $hostBufferC[$ii*$m*$n+$i].",";
                if($hostBufferC[$ii*$m*$n+$i]!=$testTruesR[$ii*$m*$n+$i]) {
                    $equals = false;
                    break;
                }
            }
            #echo "\n";
            if($equals==false) {
                break;
            }
        }
        $this->assertTrue($equals);
    }
}