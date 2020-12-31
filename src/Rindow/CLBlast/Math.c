#include <php.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_exceptions.h>
#include <ext/spl/spl_iterators.h>
#include <ext/spl/spl_exceptions.h>
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/opencl.h>
#include <clblast_c.h>
#include <Interop/Polite/Math/Matrix.h>
#include <Rindow/OpenCL/Buffer.h>
#include <Rindow/OpenCL/CommandQueue.h>
#include <Rindow/OpenCL/EventList.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_rindow_clblast.h"

#define PHP_RINDOW_CLBLAST_MATH_CONST_CROSS_CORRELATION 151
#define PHP_RINDOW_CLBLAST_MATH_CONST_CONVOLUTION       152

static zend_object_handlers rindow_clblast_math_object_handlers;

// destractor
static void php_rindow_clblast_math_free_object(zend_object* object)
{
    zend_object_std_dtor(object);
}

// constructor
static zend_object* php_rindow_clblast_math_create_object(zend_class_entry* class_type) /* {{{ */
{
    zend_object* intern = NULL;

    intern = (zend_object*)ecalloc(1, sizeof(zend_object) + zend_object_properties_size(class_type));

    zend_object_std_init(intern, class_type);
    object_properties_init(intern, class_type);

    intern->handlers = &rindow_clblast_math_object_handlers;

    return intern;
} /* }}} */

/* Method Rindow\CLBlast\Math::
    public function sum(
        int $n,
        Buffer $result, int $offsetResult,
        Buffer $X, int $offsetX, int $incX,
        CommandQueue $queue, EventList $event
    ) : void
 {{{ */
static PHP_METHOD(Math, sum)
{
    zend_long n;
    zval* result_obj_p;
    zend_long offsetR;
    zval* x_obj_p;
    zend_long offsetX;
    zend_long incX;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferX;
    php_rindow_opencl_buffer_t* bufferR;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 7, 8)
        Z_PARAM_LONG(n)
        Z_PARAM_OBJECT(result_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetR)
        Z_PARAM_OBJECT(x_obj_p)       // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    bufferR = Z_RINDOW_OPENCL_BUFFER_OBJ_P(result_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferR,"result")) {
        return;
    }
    bufferX = Z_RINDOW_OPENCL_BUFFER_OBJ_P(x_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferX,"x")) {
        return;
    }
    queue_obj = Z_RINDOW_OPENCL_COMMAND_QUEUE_OBJ_P(queue_obj_p);
    if(php_rindow_clblast_assert_opencl_command_queue_type(queue_obj,"queue")) {
        return;
    }
    if(event_obj_p && Z_TYPE_P(event_obj_p) == IS_OBJECT) {
        event_obj = Z_RINDOW_OPENCL_EVENT_LIST_OBJ_P(event_obj_p);
        if(php_rindow_clblast_assert_opencl_event_list_type(event_obj,"events")) {
            return;
        }
        event_p = &event;
    }

    switch (bufferX->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSsum((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDsum((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlast?sum error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Math::
    public function imax(
        int $n,
        Buffer $result, int $offsetResult,
        Buffer $X, int $offsetX, int $incX,
        CommandQueue $queue, EventList $event
    ) : void
 {{{ */
static PHP_METHOD(Math, imax)
{
    zend_long n;
    zval* result_obj_p;
    zend_long offsetR;
    zval* x_obj_p;
    zend_long offsetX;
    zend_long incX;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferX;
    php_rindow_opencl_buffer_t* bufferR;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 7, 8)
        Z_PARAM_LONG(n)
        Z_PARAM_OBJECT(result_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetR)
        Z_PARAM_OBJECT(x_obj_p)       // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    bufferR = Z_RINDOW_OPENCL_BUFFER_OBJ_P(result_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferR,"result")) {
        return;
    }
    bufferX = Z_RINDOW_OPENCL_BUFFER_OBJ_P(x_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferX,"x")) {
        return;
    }
    queue_obj = Z_RINDOW_OPENCL_COMMAND_QUEUE_OBJ_P(queue_obj_p);
    if(php_rindow_clblast_assert_opencl_command_queue_type(queue_obj,"queue")) {
        return;
    }
    if(event_obj_p && Z_TYPE_P(event_obj_p) == IS_OBJECT) {
        event_obj = Z_RINDOW_OPENCL_EVENT_LIST_OBJ_P(event_obj_p);
        if(php_rindow_clblast_assert_opencl_event_list_type(event_obj,"events")) {
            return;
        }
        event_p = &event;
    }
    switch (bufferX->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastiSmax((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastiDmax((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?max error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Math::
public function imin(
    int $n,
    Buffer $result, int $offsetResult,
    Buffer $X, int $offsetX, int $incX,
    CommandQueue $queue, EventList $event
) : void
 {{{ */
static PHP_METHOD(Math, imin)
{
    zend_long n;
    zval* result_obj_p;
    zend_long offsetR;
    zval* x_obj_p;
    zend_long offsetX;
    zend_long incX;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferX;
    php_rindow_opencl_buffer_t* bufferR;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 7, 8)
        Z_PARAM_LONG(n)
        Z_PARAM_OBJECT(result_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetR)
        Z_PARAM_OBJECT(x_obj_p)       // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    bufferR = Z_RINDOW_OPENCL_BUFFER_OBJ_P(result_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferR,"result")) {
        return;
    }
    bufferX = Z_RINDOW_OPENCL_BUFFER_OBJ_P(x_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferX,"x")) {
        return;
    }
    queue_obj = Z_RINDOW_OPENCL_COMMAND_QUEUE_OBJ_P(queue_obj_p);
    if(php_rindow_clblast_assert_opencl_command_queue_type(queue_obj,"queue")) {
        return;
    }
    if(event_obj_p && Z_TYPE_P(event_obj_p) == IS_OBJECT) {
        event_obj = Z_RINDOW_OPENCL_EVENT_LIST_OBJ_P(event_obj_p);
        if(php_rindow_clblast_assert_opencl_event_list_type(event_obj,"events")) {
            return;
        }
        event_p = &event;
    }
    switch (bufferX->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastiSmin((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastiDmin((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?amin error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Math::
    public function hadamard(
        int $n,
        float $alpha,
        Buffer $X, int $offsetX, int $incX,
        Buffer $Y, int $offsetY, int $incY,
        float $beta,
        Buffer $Z, int $offsetZ, int $incZ,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Math, hadamard)
{
    zend_long n;
    double alpha;
    zval* x_obj_p;
    zend_long offsetX;
    zend_long incX;
    zval* y_obj_p;
    zend_long offsetY;
    zend_long incY;
    double beta;
    zval* z_obj_p;
    zend_long offsetZ;
    zend_long incZ;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferX;
    php_rindow_opencl_buffer_t* bufferY;
    php_rindow_opencl_buffer_t* bufferZ;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 13, 14)
        Z_PARAM_LONG(n)
        Z_PARAM_DOUBLE(alpha)
        Z_PARAM_OBJECT(x_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_OBJECT(y_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetY)
        Z_PARAM_LONG(incY)
        Z_PARAM_DOUBLE(beta)
        Z_PARAM_OBJECT(z_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetZ)
        Z_PARAM_LONG(incZ)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    bufferX = Z_RINDOW_OPENCL_BUFFER_OBJ_P(x_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferX,"x")) {
        return;
    }
    bufferY = Z_RINDOW_OPENCL_BUFFER_OBJ_P(y_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferY,"y")) {
        return;
    }
    bufferZ = Z_RINDOW_OPENCL_BUFFER_OBJ_P(z_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferZ,"z")) {
        return;
    }
    queue_obj = Z_RINDOW_OPENCL_COMMAND_QUEUE_OBJ_P(queue_obj_p);
    if(php_rindow_clblast_assert_opencl_command_queue_type(queue_obj,"queue")) {
        return;
    }
    if(event_obj_p && Z_TYPE_P(event_obj_p) == IS_OBJECT) {
        event_obj = Z_RINDOW_OPENCL_EVENT_LIST_OBJ_P(event_obj_p);
        if(php_rindow_clblast_assert_opencl_event_list_type(event_obj,"events")) {
            return;
        }
        event_p = &event;
    }

    // Check Buffer X and Y
    if(bufferX->dtype!=bufferY->dtype) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for X and Y", 0);
        return;
    }

    switch (bufferX->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastShad((size_t)n, (float)alpha,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                (float)beta,
                bufferZ->buffer, (size_t)offsetZ, (size_t)incZ,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDhad((size_t)n, (double)alpha,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                (double)beta,
                bufferZ->buffer, (size_t)offsetZ, (size_t)incZ,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlast?had error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Math::
    public function omatcopy(
        int $order,
        int $transA,
        int $m,
        int $n,
        float $alpha,
        Buffer $A, int $offsetA, int $ldA,
        Buffer $B, int $offsetB, int $ldB,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Math, omatcopy)
{
    zend_long order;
    zend_long transA;
    zend_long m;
    zend_long n;
    double alpha;
    zval* a_obj_p;
    zend_long offsetA;
    zend_long ldA;
    zval* b_obj_p;
    zend_long offsetB;
    zend_long ldB;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferA;
    php_rindow_opencl_buffer_t* bufferB;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 12, 13)
        Z_PARAM_LONG(order)
        Z_PARAM_LONG(transA)
        Z_PARAM_LONG(m)
        Z_PARAM_LONG(n)
        Z_PARAM_DOUBLE(alpha)
        Z_PARAM_OBJECT(a_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetA)
        Z_PARAM_LONG(ldA)
        Z_PARAM_OBJECT(b_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetB)
        Z_PARAM_LONG(ldB)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    bufferA = Z_RINDOW_OPENCL_BUFFER_OBJ_P(a_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferA,"a")) {
        return;
    }
    bufferB = Z_RINDOW_OPENCL_BUFFER_OBJ_P(b_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferB,"b")) {
        return;
    }
    queue_obj = Z_RINDOW_OPENCL_COMMAND_QUEUE_OBJ_P(queue_obj_p);
    if(php_rindow_clblast_assert_opencl_command_queue_type(queue_obj,"queue")) {
        return;
    }
    if(event_obj_p && Z_TYPE_P(event_obj_p) == IS_OBJECT) {
        event_obj = Z_RINDOW_OPENCL_EVENT_LIST_OBJ_P(event_obj_p);
        if(php_rindow_clblast_assert_opencl_event_list_type(event_obj,"events")) {
            return;
        }
        event_p = &event;
    }

    if(bufferA->dtype!=bufferB->dtype) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for A and B", 0);
        return;
    }

    switch (bufferA->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSomatcopy(
                (CLBlastLayout)order,
                (CLBlastTranspose)transA,
                (size_t)m,(size_t)n,
                (float)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                bufferB->buffer, (size_t)offsetB, (size_t)ldB,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDomatcopy(
                (CLBlastLayout)order,
                (CLBlastTranspose)transA,
                (size_t)m,(size_t)n,
                (double)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                bufferB->buffer, (size_t)offsetB, (size_t)ldB,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?omatcopy error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */


/* Method Rindow\CLBlast\Math::
    public function im2col(
        int $kernel_mode,
        int $channels, int $height, int $width,
        int $kernel_h, int $kernel_w,
        int $pad_h, int $pad_w,
        int $stride_h, int $stride_w,
        int $dilation_h, int $dilation_w,
        Buffer $im_buffer, int $im_offset,
        Buffer $col_buffer, int $col_offset,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Math, im2col)
{
    zend_long kernel_mode;
    zend_long channels;
    zend_long height;
    zend_long width;
    zend_long kernel_h;
    zend_long kernel_w;
    zend_long pad_h;
    zend_long pad_w;
    zend_long stride_h;
    zend_long stride_w;
    zend_long dilation_h;
    zend_long dilation_w;
    zval* im_buffer_obj_p;
    zend_long im_offset;
    zval* col_buffer_obj_p;
    zend_long col_offset;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* im_buffer_obj;
    php_rindow_opencl_buffer_t* col_buffer_obj;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 17, 18)
        Z_PARAM_LONG(kernel_mode)
        Z_PARAM_LONG(channels)
        Z_PARAM_LONG(height)
        Z_PARAM_LONG(width)
        Z_PARAM_LONG(kernel_h)
        Z_PARAM_LONG(kernel_w)
        Z_PARAM_LONG(pad_h)
        Z_PARAM_LONG(pad_w)
        Z_PARAM_LONG(stride_h)
        Z_PARAM_LONG(stride_w)
        Z_PARAM_LONG(dilation_h)
        Z_PARAM_LONG(dilation_w)
        Z_PARAM_OBJECT(im_buffer_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(im_offset)
        Z_PARAM_OBJECT(col_buffer_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(col_offset)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    im_buffer_obj = Z_RINDOW_OPENCL_BUFFER_OBJ_P(im_buffer_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(im_buffer_obj,"im_buffer")) {
        return;
    }
    col_buffer_obj = Z_RINDOW_OPENCL_BUFFER_OBJ_P(col_buffer_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(col_buffer_obj,"col_buffer")) {
        return;
    }
    queue_obj = Z_RINDOW_OPENCL_COMMAND_QUEUE_OBJ_P(queue_obj_p);
    if(php_rindow_clblast_assert_opencl_command_queue_type(queue_obj,"queue")) {
        return;
    }
    if(event_obj_p && Z_TYPE_P(event_obj_p) == IS_OBJECT) {
        event_obj = Z_RINDOW_OPENCL_EVENT_LIST_OBJ_P(event_obj_p);
        if(php_rindow_clblast_assert_opencl_event_list_type(event_obj,"events")) {
            return;
        }
        event_p = &event;
    }

    if(im_buffer_obj->dtype!=col_buffer_obj->dtype) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for im_buffer and col_buffer", 0);
        return;
    }

    switch (im_buffer_obj->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSim2col(
                (CLBlastKernelMode)kernel_mode,
                (size_t)channels,(size_t)height,(size_t)width,
                (size_t)kernel_h,(size_t)kernel_w,
                (size_t)pad_h,(size_t)pad_h,
                (size_t)stride_h,(size_t)stride_h,
                (size_t)dilation_h,(size_t)dilation_w,
                im_buffer_obj->buffer, (size_t)im_offset,
                col_buffer_obj->buffer, (size_t)col_offset,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDim2col(
                (CLBlastKernelMode)kernel_mode,
                (size_t)channels,(size_t)height,(size_t)width,
                (size_t)kernel_h,(size_t)kernel_w,
                (size_t)pad_h,(size_t)pad_h,
                (size_t)stride_h,(size_t)stride_h,
                (size_t)dilation_h,(size_t)dilation_w,
                im_buffer_obj->buffer, (size_t)im_offset,
                col_buffer_obj->buffer, (size_t)col_offset,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?im2col error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Math::
    public function col2im(
        int $kernel_mode,
        int $channels, int $height, int $width,
        int $kernel_h, int $kernel_w,
        int $pad_h, int $pad_w,
        int $stride_h, int $stride_w,
        int $dilation_h, int $dilation_w,
        Buffer $col_buffer, int $col_offset,
        Buffer $im_buffer, int $im_offset,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Math, col2im)
{
    zend_long kernel_mode;
    zend_long channels;
    zend_long height;
    zend_long width;
    zend_long kernel_h;
    zend_long kernel_w;
    zend_long pad_h;
    zend_long pad_w;
    zend_long stride_h;
    zend_long stride_w;
    zend_long dilation_h;
    zend_long dilation_w;
    zval* col_buffer_obj_p;
    zend_long col_offset;
    zval* im_buffer_obj_p;
    zend_long im_offset;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* col_buffer_obj;
    php_rindow_opencl_buffer_t* im_buffer_obj;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 17, 18)
        Z_PARAM_LONG(kernel_mode)
        Z_PARAM_LONG(channels)
        Z_PARAM_LONG(height)
        Z_PARAM_LONG(width)
        Z_PARAM_LONG(kernel_h)
        Z_PARAM_LONG(kernel_w)
        Z_PARAM_LONG(pad_h)
        Z_PARAM_LONG(pad_w)
        Z_PARAM_LONG(stride_h)
        Z_PARAM_LONG(stride_w)
        Z_PARAM_LONG(dilation_h)
        Z_PARAM_LONG(dilation_w)
        Z_PARAM_OBJECT(col_buffer_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(col_offset)
        Z_PARAM_OBJECT(im_buffer_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(im_offset)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    im_buffer_obj = Z_RINDOW_OPENCL_BUFFER_OBJ_P(im_buffer_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(im_buffer_obj,"im_buffer")) {
        return;
    }
    col_buffer_obj = Z_RINDOW_OPENCL_BUFFER_OBJ_P(col_buffer_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(col_buffer_obj,"col_buffer")) {
        return;
    }
    queue_obj = Z_RINDOW_OPENCL_COMMAND_QUEUE_OBJ_P(queue_obj_p);
    if(php_rindow_clblast_assert_opencl_command_queue_type(queue_obj,"queue")) {
        return;
    }
    if(event_obj_p && Z_TYPE_P(event_obj_p) == IS_OBJECT) {
        event_obj = Z_RINDOW_OPENCL_EVENT_LIST_OBJ_P(event_obj_p);
        if(php_rindow_clblast_assert_opencl_event_list_type(event_obj,"events")) {
            return;
        }
        event_p = &event;
    }

    if(im_buffer_obj->dtype!=col_buffer_obj->dtype) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for im_buffer and col_buffer", 0);
        return;
    }

    switch (im_buffer_obj->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastScol2im(
                (CLBlastKernelMode)kernel_mode,
                (size_t)channels,(size_t)height,(size_t)width,
                (size_t)kernel_h,(size_t)kernel_w,
                (size_t)pad_h,(size_t)pad_h,
                (size_t)stride_h,(size_t)stride_h,
                (size_t)dilation_h,(size_t)dilation_w,
                col_buffer_obj->buffer, (size_t)col_offset,
                im_buffer_obj->buffer, (size_t)im_offset,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDcol2im(
                (CLBlastKernelMode)kernel_mode,
                (size_t)channels,(size_t)height,(size_t)width,
                (size_t)kernel_h,(size_t)kernel_w,
                (size_t)pad_h,(size_t)pad_h,
                (size_t)stride_h,(size_t)stride_h,
                (size_t)dilation_h,(size_t)dilation_w,
                col_buffer_obj->buffer, (size_t)col_offset,
                im_buffer_obj->buffer, (size_t)im_offset,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?col2im error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Math::
    public function convgemm(
        int $kernel_mode,
        int $channels, int $height, int $width,
        int $kernel_h, int $kernel_w,
        int $pad_h, int $pad_w,
        int $stride_h, int $stride_w,
        int $dilation_h, int $dilation_w,
        int num_kernels,
        int batch_count,
        Buffer $im_buffer, int $im_offset,
        Buffer kernel_buffer, int kernel_offset,
        Buffer result_buffer, int result_offset,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Math, convgemm)
{
    zend_long kernel_mode;
    zend_long channels;
    zend_long height;
    zend_long width;
    zend_long kernel_h;
    zend_long kernel_w;
    zend_long pad_h;
    zend_long pad_w;
    zend_long stride_h;
    zend_long stride_w;
    zend_long dilation_h;
    zend_long dilation_w;
    zend_long num_kernels;
    zend_long batch_count;
    zval* im_buffer_obj_p;
    zend_long im_offset;
    zval* kernel_buffer_obj_p;
    zend_long kernel_offset;
    zval* result_buffer_obj_p;
    zend_long result_offset;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* im_buffer_obj;
    php_rindow_opencl_buffer_t* kernel_buffer_obj;
    php_rindow_opencl_buffer_t* result_buffer_obj;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 21, 22)
        Z_PARAM_LONG(kernel_mode)
        Z_PARAM_LONG(channels)
        Z_PARAM_LONG(height)
        Z_PARAM_LONG(width)
        Z_PARAM_LONG(kernel_h)
        Z_PARAM_LONG(kernel_w)
        Z_PARAM_LONG(pad_h)
        Z_PARAM_LONG(pad_w)
        Z_PARAM_LONG(stride_h)
        Z_PARAM_LONG(stride_w)
        Z_PARAM_LONG(dilation_h)
        Z_PARAM_LONG(dilation_w)
        Z_PARAM_LONG(num_kernels)
        Z_PARAM_LONG(batch_count)
        Z_PARAM_OBJECT(im_buffer_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(im_offset)
        Z_PARAM_OBJECT(kernel_buffer_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(kernel_offset)
        Z_PARAM_OBJECT(result_buffer_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(result_offset)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    im_buffer_obj = Z_RINDOW_OPENCL_BUFFER_OBJ_P(im_buffer_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(im_buffer_obj,"im_buffer")) {
        return;
    }
    kernel_buffer_obj = Z_RINDOW_OPENCL_BUFFER_OBJ_P(kernel_buffer_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(kernel_buffer_obj,"kernel_buffer")) {
        return;
    }
    result_buffer_obj = Z_RINDOW_OPENCL_BUFFER_OBJ_P(result_buffer_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(result_buffer_obj,"result_buffer")) {
        return;
    }
    queue_obj = Z_RINDOW_OPENCL_COMMAND_QUEUE_OBJ_P(queue_obj_p);
    if(php_rindow_clblast_assert_opencl_command_queue_type(queue_obj,"queue")) {
        return;
    }
    if(event_obj_p && Z_TYPE_P(event_obj_p) == IS_OBJECT) {
        event_obj = Z_RINDOW_OPENCL_EVENT_LIST_OBJ_P(event_obj_p);
        if(php_rindow_clblast_assert_opencl_event_list_type(event_obj,"events")) {
            return;
        }
        event_p = &event;
    }

    if(im_buffer_obj->dtype!=kernel_buffer_obj->dtype) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for im_buffer and kernel_buffer", 0);
        return;
    }

    switch (im_buffer_obj->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSconvgemm(
                (CLBlastKernelMode)kernel_mode,
                (size_t)channels,(size_t)height,(size_t)width,
                (size_t)kernel_h,(size_t)kernel_w,
                (size_t)pad_h,(size_t)pad_h,
                (size_t)stride_h,(size_t)stride_h,
                (size_t)dilation_h,(size_t)dilation_w,
                (size_t)num_kernels, (size_t)batch_count,
                im_buffer_obj->buffer, (size_t)im_offset,
                kernel_buffer_obj->buffer, (size_t)kernel_offset,
                result_buffer_obj->buffer, (size_t)result_offset,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDconvgemm(
                (CLBlastKernelMode)kernel_mode,
                (size_t)channels,(size_t)height,(size_t)width,
                (size_t)kernel_h,(size_t)kernel_w,
                (size_t)pad_h,(size_t)pad_h,
                (size_t)stride_h,(size_t)stride_h,
                (size_t)dilation_h,(size_t)dilation_w,
                (size_t)num_kernels, (size_t)batch_count,
                im_buffer_obj->buffer, (size_t)im_offset,
                kernel_buffer_obj->buffer, (size_t)kernel_offset,
                result_buffer_obj->buffer, (size_t)result_offset,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?convgemm error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Math::
    public function axpyBatched(
        int $n,
        LinearBuffer $alpha, int $offsetA,
        Buffer $X, LinearBuffer $offsetsX, $offsetX, int $incX,
        Buffer $Y, LinearBuffer $offsetsY, $offsetY, int $incY,
        int $batch_count,
        CommandQueue $queue, EventList $event
     ) : void
{{{ */
static PHP_METHOD(Math, axpyBatched)
{
    zend_long n;
    zval* alpha_obj_p;
    zend_long offsetA;
    zval* x_obj_p;
    zval* offsetsX_obj_p;
    zend_long offsetX;
    zend_long incX;
    zval* y_obj_p;
    zval* offsetsY_obj_p;
    zend_long offsetY;
    zend_long incY;
    zend_long batch_count;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_interop_polite_math_matrix_linear_buffer_t* alpha_obj;
    php_rindow_opencl_buffer_t* bufferX;
    php_interop_polite_math_matrix_linear_buffer_t* offsetsX_obj;
    php_rindow_opencl_buffer_t* bufferY;
    php_interop_polite_math_matrix_linear_buffer_t* offsetsY_obj;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 13, 14)
        Z_PARAM_LONG(n)
        Z_PARAM_OBJECT(alpha_obj_p)    // Interop\Polite\Math\Matrix\LinearBuffer
        Z_PARAM_LONG(offsetA)
        Z_PARAM_OBJECT(x_obj_p)        // Rindow\OpenCL\Buffer
        Z_PARAM_OBJECT(offsetsX_obj_p) // Interop\Polite\Math\Matrix\LinearBuffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_OBJECT(y_obj_p)        // Rindow\OpenCL\Buffer
        Z_PARAM_OBJECT(offsetsY_obj_p) // Interop\Polite\Math\Matrix\LinearBuffer
        Z_PARAM_LONG(offsetY)
        Z_PARAM_LONG(incY)
        Z_PARAM_LONG(batch_count)
        Z_PARAM_OBJECT(queue_obj_p)    // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)    // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    alpha_obj = Z_INTEROP_POLITE_MATH_MATRIX_LINEAR_BUFFER_OBJ_P(alpha_obj_p);
    if(php_rindow_clblast_assert_host_buffer_type(alpha_obj,"alpha")) {
        return;
    }
    bufferX = Z_RINDOW_OPENCL_BUFFER_OBJ_P(x_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferX,"x")) {
        return;
    }
    offsetsX_obj = Z_INTEROP_POLITE_MATH_MATRIX_LINEAR_BUFFER_OBJ_P(offsetsX_obj_p);
    if(php_rindow_clblast_assert_host_buffer_type(offsetsX_obj,"offsetsX")) {
        return;
    }
    bufferY = Z_RINDOW_OPENCL_BUFFER_OBJ_P(y_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferY,"y")) {
        return;
    }
    offsetsY_obj = Z_INTEROP_POLITE_MATH_MATRIX_LINEAR_BUFFER_OBJ_P(offsetsY_obj_p);
    if(php_rindow_clblast_assert_host_buffer_type(offsetsY_obj,"offsetsY")) {
        return;
    }
    queue_obj = Z_RINDOW_OPENCL_COMMAND_QUEUE_OBJ_P(queue_obj_p);
    if(php_rindow_clblast_assert_opencl_command_queue_type(queue_obj,"queue")) {
        return;
    }
    if(event_obj_p && Z_TYPE_P(event_obj_p) == IS_OBJECT) {
        event_obj = Z_RINDOW_OPENCL_EVENT_LIST_OBJ_P(event_obj_p);
        if(php_rindow_clblast_assert_opencl_event_list_type(event_obj,"events")) {
            return;
        }
        event_p = &event;
    }

    if(offsetA<0) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetA must be greater than zero or equal", 0);
        return;
    }
    if(offsetX<0) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetX must be greater than zero or equal", 0);
        return;
    }
    if(offsetY<0) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetY must be greater than zero or equal", 0);
        return;
    }
    if(batch_count<0) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "batch_count must be greater than zero or equal", 0);
        return;
    }
    if(offsetA+batch_count>alpha_obj->size) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "alpha LinearBuffer is too small.", 0);
        return;
    }
    if(offsetX+batch_count>offsetsX_obj->size) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetsX LinearBuffer is too small.", 0);
        return;
    }
    if(offsetY+batch_count>offsetsY_obj->size) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetsY LinearBuffer is too small.", 0);
        return;
    }
    if(offsetsX_obj->dtype==!php_interop_polite_math_matrix_dtype_int64) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetsX LinearBuffer data type must be int64.", 0);
        return;
    }
    if(offsetsY_obj->dtype==!php_interop_polite_math_matrix_dtype_int64) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetsY LinearBuffer data type must be int64.", 0);
        return;
    }

    // Check Buffer X and Y
    if(bufferX->dtype!=bufferY->dtype||alpha_obj->dtype!=bufferX->dtype) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for A,X and Y", 0);
        return;
    }

    switch (bufferX->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSaxpyBatched((size_t)n, &(((float*)alpha_obj->data)[offsetA]),
                bufferX->buffer, &(((size_t*)offsetsX_obj->data)[offsetX]), (size_t)incX,
                bufferY->buffer, &(((size_t*)offsetsY_obj->data)[offsetY]), (size_t)incY,
                (size_t)batch_count,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDaxpyBatched((size_t)n, &(((double*)alpha_obj->data)[offsetA]),
                bufferX->buffer, &(((size_t*)offsetsX_obj->data)[offsetX]), (size_t)incX,
                bufferY->buffer, &(((size_t*)offsetsY_obj->data)[offsetY]), (size_t)incY,
                (size_t)batch_count,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlast?axpyBatched error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Math::
    public function gemmBatched(
        int $order,
        int $transA,
        int $transB,
        int $m,
        int $n,
        int $k,
        LinearBuffer $alpha, int $offsetAlpha,
        Buffer $A, LinearBuffer $offsetsA, int $offsetA, int $ldA,
        Buffer $B, LinearBuffer $offsetsB, int $offsetB, int $ldB,
        LinearBuffer $beta, int $offsetA,
        Buffer $C, LinearBuffer $offsetsC, int $offsetC, int $ldC,
        int $batch_count,
        CommandQueue $queue, EventList $event
    ) : void
{{{ */
static PHP_METHOD(Math, gemmBatched)
{
    zend_long order;
    zend_long transA;
    zend_long transB;
    zend_long m;
    zend_long n;
    zend_long k;
    zval* alpha_obj_p;
    zend_long offsetAlpha;
    zval* a_obj_p;
    zval* offsetsA_obj_p;
    zend_long offsetA;
    zend_long ldA;
    zval* b_obj_p;
    zval* offsetsB_obj_p;
    zend_long offsetB;
    zend_long ldB;
    zval* beta_obj_p;
    zend_long offsetBeta;
    zval* c_obj_p;
    zval* offsetsC_obj_p;
    zend_long offsetC;
    zend_long ldC;
    zend_long batch_count;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_interop_polite_math_matrix_linear_buffer_t* alpha_obj;
    php_rindow_opencl_buffer_t* bufferA;
    php_interop_polite_math_matrix_linear_buffer_t* offsetsA_obj;
    php_rindow_opencl_buffer_t* bufferB;
    php_interop_polite_math_matrix_linear_buffer_t* offsetsB_obj;
    php_interop_polite_math_matrix_linear_buffer_t* beta_obj;
    php_rindow_opencl_buffer_t* bufferC;
    php_interop_polite_math_matrix_linear_buffer_t* offsetsC_obj;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 24, 25)
        Z_PARAM_LONG(order)
        Z_PARAM_LONG(transA)
        Z_PARAM_LONG(transB)
        Z_PARAM_LONG(m)
        Z_PARAM_LONG(n)
        Z_PARAM_LONG(k)
        Z_PARAM_OBJECT(alpha_obj_p)    // Interop\Polite\Math\Matrix\LinearBuffer
        Z_PARAM_LONG(offsetAlpha)
        Z_PARAM_OBJECT(a_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_OBJECT(offsetsA_obj_p)    // Interop\Polite\Math\Matrix\LinearBuffer
        Z_PARAM_LONG(offsetA)
        Z_PARAM_LONG(ldA)
        Z_PARAM_OBJECT(b_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_OBJECT(offsetsB_obj_p)    // Interop\Polite\Math\Matrix\LinearBuffer
        Z_PARAM_LONG(offsetB)
        Z_PARAM_LONG(ldB)
        Z_PARAM_OBJECT(beta_obj_p)    // Interop\Polite\Math\Matrix\LinearBuffer
        Z_PARAM_LONG(offsetBeta)
        Z_PARAM_OBJECT(c_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_OBJECT(offsetsC_obj_p)    // Interop\Polite\Math\Matrix\LinearBuffer
        Z_PARAM_LONG(offsetC)
        Z_PARAM_LONG(ldC)
        Z_PARAM_LONG(batch_count)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    alpha_obj = Z_INTEROP_POLITE_MATH_MATRIX_LINEAR_BUFFER_OBJ_P(alpha_obj_p);
    if(php_rindow_clblast_assert_host_buffer_type(alpha_obj,"alpha")) {
        return;
    }
    bufferA = Z_RINDOW_OPENCL_BUFFER_OBJ_P(a_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferA,"a")) {
        return;
    }
    offsetsA_obj = Z_INTEROP_POLITE_MATH_MATRIX_LINEAR_BUFFER_OBJ_P(offsetsA_obj_p);
    if(php_rindow_clblast_assert_host_buffer_type(offsetsA_obj,"offsetsA")) {
        return;
    }
    bufferB = Z_RINDOW_OPENCL_BUFFER_OBJ_P(b_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferB,"b")) {
        return;
    }
    offsetsB_obj = Z_INTEROP_POLITE_MATH_MATRIX_LINEAR_BUFFER_OBJ_P(offsetsB_obj_p);
    if(php_rindow_clblast_assert_host_buffer_type(offsetsB_obj,"offsetsB")) {
        return;
    }
    beta_obj = Z_INTEROP_POLITE_MATH_MATRIX_LINEAR_BUFFER_OBJ_P(beta_obj_p);
    if(php_rindow_clblast_assert_host_buffer_type(beta_obj,"beta")) {
        return;
    }
    bufferC = Z_RINDOW_OPENCL_BUFFER_OBJ_P(c_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferC,"c")) {
        return;
    }
    offsetsC_obj = Z_INTEROP_POLITE_MATH_MATRIX_LINEAR_BUFFER_OBJ_P(offsetsC_obj_p);
    if(php_rindow_clblast_assert_host_buffer_type(offsetsC_obj,"offsetsC")) {
        return;
    }
    queue_obj = Z_RINDOW_OPENCL_COMMAND_QUEUE_OBJ_P(queue_obj_p);
    if(php_rindow_clblast_assert_opencl_command_queue_type(queue_obj,"queue")) {
        return;
    }
    if(event_obj_p && Z_TYPE_P(event_obj_p) == IS_OBJECT) {
        event_obj = Z_RINDOW_OPENCL_EVENT_LIST_OBJ_P(event_obj_p);
        if(php_rindow_clblast_assert_opencl_event_list_type(event_obj,"events")) {
            return;
        }
        event_p = &event;
    }

    if(offsetAlpha<0) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetAlpha must be greater than zero or equal", 0);
        return;
    }
    if(offsetA<0) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetA must be greater than zero or equal", 0);
        return;
    }
    if(offsetB<0) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetB must be greater than zero or equal", 0);
        return;
    }
    if(offsetBeta<0) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetBeta must be greater than zero or equal", 0);
        return;
    }
    if(offsetC<0) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetC must be greater than zero or equal", 0);
        return;
    }
    if(batch_count<0) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "batch_count must be greater than zero or equal", 0);
        return;
    }
    if(offsetAlpha+batch_count>alpha_obj->size) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "alpha LinearBuffer is too small.", 0);
        return;
    }
    if(offsetA+batch_count>offsetsA_obj->size) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetsA LinearBuffer is too small.", 0);
        return;
    }
    if(offsetB+batch_count>offsetsB_obj->size) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetsB LinearBuffer is too small.", 0);
        return;
    }
    if(offsetBeta+batch_count>beta_obj->size) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "beta LinearBuffer is too small.", 0);
        return;
    }
    if(offsetC+batch_count>offsetsC_obj->size) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetsC LinearBuffer is too small.", 0);
        return;
    }
    if(offsetsA_obj->dtype==!php_interop_polite_math_matrix_dtype_int64) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetsA LinearBuffer data type must be int64.", 0);
        return;
    }
    if(offsetsB_obj->dtype==!php_interop_polite_math_matrix_dtype_int64) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetsB LinearBuffer data type must be int64.", 0);
        return;
    }
    if(offsetsB_obj->dtype==!php_interop_polite_math_matrix_dtype_int64) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "offsetsC LinearBuffer data type must be int64.", 0);
        return;
    }

    if(bufferA->dtype!=bufferB->dtype || bufferB->dtype!=bufferC->dtype||
        bufferC->dtype!=alpha_obj->dtype||bufferC->dtype!=beta_obj->dtype) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for A,B,C,alpha and beta", 0);
        return;
    }
    switch (bufferA->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSgemmBatched(
                (CLBlastLayout)order,
                (CLBlastTranspose)transA,
                (CLBlastTranspose)transB,
                (size_t)m, (size_t)n, (size_t)k,
                &(((float*)alpha_obj->data)[offsetAlpha]),
                bufferA->buffer, &(((size_t*)offsetsA_obj->data)[offsetA]), (size_t)ldA,
                bufferB->buffer, &(((size_t*)offsetsB_obj->data)[offsetB]), (size_t)ldB,
                &(((float*)beta_obj->data)[offsetBeta]),
                bufferC->buffer, &(((size_t*)offsetsC_obj->data)[offsetC]), (size_t)ldC,
                (size_t)batch_count,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDgemmBatched(
                (CLBlastLayout)order,
                (CLBlastTranspose)transA,
                (CLBlastTranspose)transB,
                (size_t)m, (size_t)n, (size_t)k,
                &(((double*)alpha_obj->data)[offsetAlpha]),
                bufferA->buffer, &(((size_t*)offsetsA_obj->data)[offsetA]), (size_t)ldA,
                bufferB->buffer, &(((size_t*)offsetsB_obj->data)[offsetB]), (size_t)ldB,
                &(((double*)beta_obj->data)[offsetBeta]),
                bufferC->buffer, &(((size_t*)offsetsC_obj->data)[offsetC]), (size_t)ldC,
                (size_t)batch_count,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?gemmBatched error=%d", status);
        return;
    }
    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Math::
    public function gemmStridedBatched(
        int $order,
        int $transA,
        int $transB,
        int $m,
        int $n,
        int $k,
        float $alpha,
        Buffer $A, int $offsetA, int $ldA, int $strideA,
        Buffer $B, int $offsetB, int $ldB, int $strideB,
        float $beta,
        Buffer $C, int $offsetC, int $ldC, int $strideC,
        int batch_count,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Math, gemmStridedBatched)
{
    zend_long order;
    zend_long transA;
    zend_long transB;
    zend_long m;
    zend_long n;
    zend_long k;
    double alpha;
    zval* a_obj_p;
    zend_long offsetA;
    zend_long ldA;
    zend_long strideA;
    zval* b_obj_p;
    zend_long offsetB;
    zend_long ldB;
    zend_long strideB;
    double beta;
    zval* c_obj_p;
    zend_long offsetC;
    zend_long ldC;
    zend_long strideC;
    zend_long batch_count;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferA;
    php_rindow_opencl_buffer_t* bufferB;
    php_rindow_opencl_buffer_t* bufferC;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 22, 23)
        Z_PARAM_LONG(order)
        Z_PARAM_LONG(transA)
        Z_PARAM_LONG(transB)
        Z_PARAM_LONG(m)
        Z_PARAM_LONG(n)
        Z_PARAM_LONG(k)
        Z_PARAM_DOUBLE(alpha)
        Z_PARAM_OBJECT(a_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetA)
        Z_PARAM_LONG(ldA)
        Z_PARAM_LONG(strideA)
        Z_PARAM_OBJECT(b_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetB)
        Z_PARAM_LONG(ldB)
        Z_PARAM_LONG(strideB)
        Z_PARAM_DOUBLE(beta)
        Z_PARAM_OBJECT(c_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetC)
        Z_PARAM_LONG(ldC)
        Z_PARAM_LONG(strideC)
        Z_PARAM_LONG(batch_count)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    bufferA = Z_RINDOW_OPENCL_BUFFER_OBJ_P(a_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferA,"a")) {
        return;
    }
    bufferB = Z_RINDOW_OPENCL_BUFFER_OBJ_P(b_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferB,"b")) {
        return;
    }
    bufferC = Z_RINDOW_OPENCL_BUFFER_OBJ_P(c_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferC,"c")) {
        return;
    }
    queue_obj = Z_RINDOW_OPENCL_COMMAND_QUEUE_OBJ_P(queue_obj_p);
    if(php_rindow_clblast_assert_opencl_command_queue_type(queue_obj,"queue")) {
        return;
    }
    if(event_obj_p && Z_TYPE_P(event_obj_p) == IS_OBJECT) {
        event_obj = Z_RINDOW_OPENCL_EVENT_LIST_OBJ_P(event_obj_p);
        if(php_rindow_clblast_assert_opencl_event_list_type(event_obj,"events")) {
            return;
        }
        event_p = &event;
    }

    if(bufferA->dtype!=bufferB->dtype || bufferB->dtype!=bufferC->dtype) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for A and B and C", 0);
        return;
    }
    switch (bufferA->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSgemmStridedBatched(
                (CLBlastLayout)order,
                (CLBlastTranspose)transA,
                (CLBlastTranspose)transB,
                (size_t)m, (size_t)n, (size_t)k,
                (float)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA, (size_t)strideA,
                bufferB->buffer, (size_t)offsetB, (size_t)ldB, (size_t)strideB,
                (float)beta,
                bufferC->buffer, (size_t)offsetC, (size_t)ldC, (size_t)strideC,
                batch_count,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDgemmStridedBatched(
                (CLBlastLayout)order,
                (CLBlastTranspose)transA,
                (CLBlastTranspose)transB,
                (size_t)m, (size_t)n, (size_t)k,
                (double)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA, (size_t)strideA,
                bufferB->buffer, (size_t)offsetB, (size_t)ldB, (size_t)strideB,
                (double)beta,
                bufferC->buffer, (size_t)offsetC, (size_t)ldC, (size_t)strideC,
                batch_count,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?gemmStridedBatched error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */


ZEND_BEGIN_ARG_INFO_EX(ai_Math_sum, 0, 0, 7)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, result, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetR)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Math_imax, 0, 0, 7)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, result, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetR)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Math_imin, 0, 0, 7)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, result, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetR)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Math_hadamard, 0, 0, 13)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_INFO(0, alpha)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, y, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetY)
    ZEND_ARG_INFO(0, incY)
    ZEND_ARG_INFO(0, beta)
    ZEND_ARG_OBJ_INFO(0, z, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetZ)
    ZEND_ARG_INFO(0, incZ)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Math_omatcopy, 0, 0, 12)
    ZEND_ARG_INFO(0, order)
    ZEND_ARG_INFO(0, transA)
    ZEND_ARG_INFO(0, m)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_INFO(0, alpha)
    ZEND_ARG_OBJ_INFO(0, a, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetA)
    ZEND_ARG_INFO(0, ldA)
    ZEND_ARG_OBJ_INFO(0, b, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetB)
    ZEND_ARG_INFO(0, ldB)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Math_im2col, 0, 0, 17)
    ZEND_ARG_INFO(0, kernel_mode)
    ZEND_ARG_INFO(0, channels)
    ZEND_ARG_INFO(0, height)
    ZEND_ARG_INFO(0, width)
    ZEND_ARG_INFO(0, kernel_h)
    ZEND_ARG_INFO(0, kernel_w)
    ZEND_ARG_INFO(0, pad_h)
    ZEND_ARG_INFO(0, pad_w)
    ZEND_ARG_INFO(0, stride_h)
    ZEND_ARG_INFO(0, stride_w)
    ZEND_ARG_INFO(0, dilation_h)
    ZEND_ARG_INFO(0, dilation_w)
    ZEND_ARG_OBJ_INFO(0, im_buffer, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, im_offset)
    ZEND_ARG_OBJ_INFO(0, col_buffer, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, col_offset)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Math_col2im, 0, 0, 17)
    ZEND_ARG_INFO(0, kernel_mode)
    ZEND_ARG_INFO(0, channels)
    ZEND_ARG_INFO(0, height)
    ZEND_ARG_INFO(0, width)
    ZEND_ARG_INFO(0, kernel_h)
    ZEND_ARG_INFO(0, kernel_w)
    ZEND_ARG_INFO(0, pad_h)
    ZEND_ARG_INFO(0, pad_w)
    ZEND_ARG_INFO(0, stride_h)
    ZEND_ARG_INFO(0, stride_w)
    ZEND_ARG_INFO(0, dilation_h)
    ZEND_ARG_INFO(0, dilation_w)
    ZEND_ARG_OBJ_INFO(0, col_buffer, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, col_offset)
    ZEND_ARG_OBJ_INFO(0, im_buffer, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, im_offset)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Math_convgemm, 0, 0, 21)
    ZEND_ARG_INFO(0, kernel_mode)
    ZEND_ARG_INFO(0, channels)
    ZEND_ARG_INFO(0, height)
    ZEND_ARG_INFO(0, width)
    ZEND_ARG_INFO(0, kernel_h)
    ZEND_ARG_INFO(0, kernel_w)
    ZEND_ARG_INFO(0, pad_h)
    ZEND_ARG_INFO(0, pad_w)
    ZEND_ARG_INFO(0, stride_h)
    ZEND_ARG_INFO(0, stride_w)
    ZEND_ARG_INFO(0, dilation_h)
    ZEND_ARG_INFO(0, dilation_w)
    ZEND_ARG_INFO(0, num_kernels)
    ZEND_ARG_INFO(0, batch_count)
    ZEND_ARG_OBJ_INFO(0, im_buffer, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, im_offset)
    ZEND_ARG_OBJ_INFO(0, kernel_buffer, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, kernel_offset)
    ZEND_ARG_OBJ_INFO(0, result_buffer, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, result_offset)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Math_axpyBatched, 0, 0, 13)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, alphas, Interop\\Polite\\Math\\Matrix\\LinearBuffer, 0)
    ZEND_ARG_INFO(0, offsetA)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_OBJ_INFO(0, offsetsX, Interop\\Polite\\Math\\Matrix\\LinearBuffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, y, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_OBJ_INFO(0, offsetsY, Interop\\Polite\\Math\\Matrix\\LinearBuffer, 0)
    ZEND_ARG_INFO(0, offsetY)
    ZEND_ARG_INFO(0, incY)
    ZEND_ARG_INFO(0, batch_count)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Math_gemmBatched, 0, 0, 24)
    ZEND_ARG_INFO(0, order)
    ZEND_ARG_INFO(0, transA)
    ZEND_ARG_INFO(0, transB)
    ZEND_ARG_INFO(0, m)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_INFO(0, k)
    ZEND_ARG_OBJ_INFO(0, alphas, Interop\\Polite\\Math\\Matrix\\LinearBuffer, 0)
    ZEND_ARG_INFO(0, offsetAlpha)
    ZEND_ARG_OBJ_INFO(0, a, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_OBJ_INFO(0, offsetsA, Interop\\Polite\\Math\\Matrix\\LinearBuffer, 0)
    ZEND_ARG_INFO(0, offsetA)
    ZEND_ARG_INFO(0, ldA)
    ZEND_ARG_OBJ_INFO(0, b, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_OBJ_INFO(0, offsetsB, Interop\\Polite\\Math\\Matrix\\LinearBuffer, 0)
    ZEND_ARG_INFO(0, offsetB)
    ZEND_ARG_INFO(0, ldB)
    ZEND_ARG_OBJ_INFO(0, betas, Interop\\Polite\\Math\\Matrix\\LinearBuffer, 0)
    ZEND_ARG_INFO(0, offsetBeta)
    ZEND_ARG_OBJ_INFO(0, c, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_OBJ_INFO(0, offsetsC, Interop\\Polite\\Math\\Matrix\\LinearBuffer, 0)
    ZEND_ARG_INFO(0, offsetC)
    ZEND_ARG_INFO(0, ldC)
    ZEND_ARG_INFO(0, batch_count)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Math_gemmStridedBatched, 0, 0, 22)
    ZEND_ARG_INFO(0, order)
    ZEND_ARG_INFO(0, transA)
    ZEND_ARG_INFO(0, transB)
    ZEND_ARG_INFO(0, m)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_INFO(0, k)
    ZEND_ARG_INFO(0, alpha)
    ZEND_ARG_OBJ_INFO(0, a, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetA)
    ZEND_ARG_INFO(0, ldA)
    ZEND_ARG_INFO(0, strideA)
    ZEND_ARG_OBJ_INFO(0, b, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetB)
    ZEND_ARG_INFO(0, ldB)
    ZEND_ARG_INFO(0, strideB)
    ZEND_ARG_INFO(0, beta)
    ZEND_ARG_OBJ_INFO(0, c, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetC)
    ZEND_ARG_INFO(0, ldC)
    ZEND_ARG_INFO(0, strideC)
    ZEND_ARG_INFO(0, batch_count)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

/* {{{ Rindow\CLBlast\Math function entries */
static zend_function_entry php_rindow_clblast_math_me[] = {
    /* clang-format off */
    PHP_ME(Math, sum,      ai_Math_sum,      ZEND_ACC_PUBLIC)
    PHP_ME(Math, imax,     ai_Math_imax,     ZEND_ACC_PUBLIC)
    PHP_ME(Math, imin,     ai_Math_imin,     ZEND_ACC_PUBLIC)
    PHP_ME(Math, hadamard, ai_Math_hadamard, ZEND_ACC_PUBLIC)
    PHP_ME(Math, omatcopy, ai_Math_omatcopy, ZEND_ACC_PUBLIC)
    PHP_ME(Math, im2col,   ai_Math_im2col,   ZEND_ACC_PUBLIC)
    PHP_ME(Math, col2im,   ai_Math_col2im,   ZEND_ACC_PUBLIC)
    PHP_ME(Math, convgemm, ai_Math_convgemm, ZEND_ACC_PUBLIC)
    PHP_ME(Math, axpyBatched, ai_Math_axpyBatched, ZEND_ACC_PUBLIC)
    PHP_ME(Math, gemmBatched, ai_Math_gemmBatched, ZEND_ACC_PUBLIC)
    PHP_ME(Math, gemmStridedBatched, ai_Math_gemmStridedBatched, ZEND_ACC_PUBLIC)
    PHP_FE_END
    /* clang-format on */
};
/* }}} */

/* Class Rindow\CLBlast\Math {{{ */
static zend_class_entry* rindow_clblast_math_ce;

void php_rindow_clblast_math_init_ce(INIT_FUNC_ARGS)
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "Rindow\\CLBlast", "Math", php_rindow_clblast_math_me);
    rindow_clblast_math_ce = zend_register_internal_class(&ce);
    rindow_clblast_math_ce->create_object = php_rindow_clblast_math_create_object;

    memcpy(&rindow_clblast_math_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    rindow_clblast_math_object_handlers.offset    = 0;
    rindow_clblast_math_object_handlers.free_obj  = php_rindow_clblast_math_free_object;
    rindow_clblast_math_object_handlers.clone_obj = NULL;
    zend_declare_class_constant_long(rindow_clblast_math_ce, ZEND_STRL("CROSS_CORRELATION"), PHP_RINDOW_CLBLAST_MATH_CONST_CROSS_CORRELATION);
    zend_declare_class_constant_long(rindow_clblast_math_ce, ZEND_STRL("CONVOLUTION"), PHP_RINDOW_CLBLAST_MATH_CONST_CONVOLUTION);

    //zend_class_implements(rindow_clblast_math_ce, 2, spl_ce_ArrayAccess, spl_ce_Countable);
}
/* }}} */
