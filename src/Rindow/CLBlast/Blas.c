#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <php.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_exceptions.h>
#include <ext/spl/spl_iterators.h>
#include <ext/spl/spl_exceptions.h>
#include <CL/opencl.h>
#include <clblast_c.h>
#include <Interop/Polite/Math/Matrix.h>
#include <Rindow/OpenCL/Buffer.h>
#include <Rindow/OpenCL/CommandQueue.h>
#include <Rindow/OpenCL/EventList.h>


#include "php_rindow_clblast.h"

static unsigned int php_rindow_clblast_copy_buffer(
    size_t n,
    php_rindow_opencl_buffer_t* bufferX, size_t offsetX, size_t incX,
    php_rindow_opencl_buffer_t* bufferY, size_t offsetY, size_t incY,
    cl_command_queue queue, cl_event *event_p)
{
    cl_int status;
    if(incX!=1||incY!=1) {
        zend_throw_exception(spl_ce_RuntimeException, "clEnqueueCopyBuffer not supports incX and incY.", 0);
        return -1;
    }
    status = clEnqueueCopyBuffer(
        queue,
        bufferX->buffer,
        bufferY->buffer,
        bufferX->value_size*offsetX,
        bufferY->value_size*offsetY,
        bufferX->value_size*n,
        0, // num_events_in_wait_list
        NULL, // event_wait_list
        event_p);
    if(status) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "clEnqueueCopyBuffer error=%d.", status);
        return status;
    }
    return 0;
}

static zend_object_handlers rindow_clblast_blas_object_handlers;

// destractor
static void php_rindow_clblast_blas_free_object(zend_object* object)
{
    zend_object_std_dtor(object);
}

// constructor
static zend_object* php_rindow_clblast_blas_create_object(zend_class_entry* class_type) /* {{{ */
{
    zend_object* intern = NULL;

    intern = (zend_object*)ecalloc(1, sizeof(zend_object) + zend_object_properties_size(class_type));

    zend_object_std_init(intern, class_type);
    object_properties_init(intern, class_type);

    intern->handlers = &rindow_clblast_blas_object_handlers;

    return intern;
} /* }}} */

/* Method Rindow\CLBlast\Blas::
    public function scal(
        int $n,
        float $alpha,
        Buffer $X, int $offsetX, int $incX,
        CommandQueue $queue, EventList $event=NULL
    ) : void
 {{{ */
static PHP_METHOD(Blas, scal)
{
    zend_long n;
    double alpha;
    zval* x_obj_p;
    zend_long offsetX;
    zend_long incX;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferX;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 6, 7)
        Z_PARAM_LONG(n)
        Z_PARAM_DOUBLE(alpha)
        Z_PARAM_OBJECT(x_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

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
            status = CLBlastSscal((size_t)n, (float)alpha,
                        bufferX->buffer, (size_t)offsetX, (size_t)incX,
                        &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDscal((size_t)n, (double)alpha,
                        bufferX->buffer, (size_t)offsetX, (size_t)incX,
                        &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlast?scal error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function axpy(
        int $n,
        float $alpha,
        Buffer $X, int $offsetX, int $incX,
        Buffer $Y, int $offsetY, int $incY,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Blas, axpy)
{
    zend_long n;
    double alpha;
    zval* x_obj_p;
    zend_long offsetX;
    zend_long incX;
    zval* y_obj_p;
    zend_long offsetY;
    zend_long incY;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferX;
    php_rindow_opencl_buffer_t* bufferY;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 9, 10)
        Z_PARAM_LONG(n)
        Z_PARAM_DOUBLE(alpha)
        Z_PARAM_OBJECT(x_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_OBJECT(y_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetY)
        Z_PARAM_LONG(incY)
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
            status = CLBlastSaxpy((size_t)n, (float)alpha,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDaxpy((size_t)n, (double)alpha,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlast?axpy error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function dot(
        int $n,
        Buffer $result, int $offsetResult,
        Buffer $X, int $offsetX, int $incX,
        Buffer $Y, int $offsetY, int $incY,
        CommandQueue $queue, EventList $event
    ) : void
 {{{ */
static PHP_METHOD(Blas, dot)
{
    zend_long n;
    zval* result_obj_p;
    zend_long offsetR;
    zval* x_obj_p;
    zend_long offsetX;
    zend_long incX;
    zval* y_obj_p;
    zend_long offsetY;
    zend_long incY;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferR;
    php_rindow_opencl_buffer_t* bufferX;
    php_rindow_opencl_buffer_t* bufferY;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 10, 11)
        Z_PARAM_LONG(n)
        Z_PARAM_OBJECT(result_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetR)
        Z_PARAM_OBJECT(x_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_OBJECT(y_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetY)
        Z_PARAM_LONG(incY)
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
    bufferY = Z_RINDOW_OPENCL_BUFFER_OBJ_P(y_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferY,"y")) {
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
            status = CLBlastSdot((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDdot((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlast?dot error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function asum(
        int $n,
        Buffer $result, int $offsetResult,
        Buffer $X, int $offsetX, int $incX,
        CommandQueue $queue, EventList $event
    ) : void
 {{{ */
static PHP_METHOD(Blas, asum)
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
            status = CLBlastSasum((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDasum((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlast?asum error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */


/* Method Rindow\CLBlast\Blas::
    public function iamax(
        int $n,
        Buffer $result, int $offsetResult,
        Buffer $X, int $offsetX, int $incX,
        CommandQueue $queue, EventList $event
    ) : void
 {{{ */
static PHP_METHOD(Blas, iamax)
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
            status = CLBlastiSamax((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastiDamax((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?amax error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
public function iamin(
    int $n,
    Buffer $result, int $offsetResult,
    Buffer $X, int $offsetX, int $incX,
    CommandQueue $queue, EventList $event
) : void
 {{{ */
static PHP_METHOD(Blas, iamin)
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
            status = CLBlastiSamin((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastiDamin((size_t)n,
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

/* Method Rindow\CLBlast\Blas::
    public function copy(
        int $n,
        Buffer $X, int $offsetX, int $incX,
        Buffer $Y, int $offsetY, int $incY,
        CommandQueue $queue, EventList $event
    ) : void
 {{{ */
static PHP_METHOD(Blas, copy)
{
    zend_long n;
    zval* x_obj_p;
    zend_long offsetX;
    zend_long incX;
    zval* y_obj_p;
    zend_long offsetY;
    zend_long incY;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferX;
    php_rindow_opencl_buffer_t* bufferY;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 8, 9)
        Z_PARAM_LONG(n)
        Z_PARAM_OBJECT(x_obj_p)       // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_OBJECT(y_obj_p)       // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetY)
        Z_PARAM_LONG(incY)
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
            status = CLBlastScopy((size_t)n,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDcopy((size_t)n,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            status = php_rindow_clblast_copy_buffer(
                    n,
                    bufferX, offsetX, incX,
                    bufferY, offsetY, incY,
                    queue_obj->command_queue, event_p
            );
            if(status!=0) {
                return;
            }
            break;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlast?copy error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function nrm2(
        int $n,
        Buffer $result, int $offsetResult,
        Buffer $X, int $offsetX, int $incX ,
        CommandQueue $queue, EventList $event
    ) : void
 {{{ */
static PHP_METHOD(Blas, nrm2)
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
            status = CLBlastSnrm2((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDnrm2((size_t)n,
                bufferR->buffer, (size_t)offsetR,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?nrm2 error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function swap(
        int $n,
        Buffer $x, int $offsetX, int incX,
        Buffer $y, int $offsetY, int incY,
        CommandQueue $queue, EventList $event
    ) : void
 {{{ */
static PHP_METHOD(Blas, swap)
{
    zend_long n;
    zval* x_obj_p;
    zend_long offsetX;
    zend_long incX;
    zval* y_obj_p;
    zend_long offsetY;
    zend_long incY;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferX;
    php_rindow_opencl_buffer_t* bufferY;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 8, 9)
        Z_PARAM_LONG(n)
        Z_PARAM_OBJECT(x_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_OBJECT(y_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetY)
        Z_PARAM_LONG(incY)
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
    if(bufferX->dtype!=bufferY->dtype) {
            zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for X and Y", 0);
            return;
    }

    switch (bufferX->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSswap(
                n,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDswap(
                n,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?swap error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function rotg(
        DeviceBuffer $A, int $offsetA,
        DeviceBuffer $B, int $offsetB,
        DeviceBuffer $C, int $offsetC,
        DeviceBuffer $S, int $offsetS,
        CommandQueue $queue,// Rindow\OpenCL\CommandQueue
        EventList $event=null,   // Rindow\OpenCL\EventList
        ) : void
 {{{ */
static PHP_METHOD(Blas, rotg)
{
    zval* a_obj_p;
    zend_long offsetA;
    zval* b_obj_p;
    zend_long offsetB;
    zval* c_obj_p;
    zend_long offsetC;
    zval* s_obj_p;
    zend_long offsetS;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferA;
    php_rindow_opencl_buffer_t* bufferB;
    php_rindow_opencl_buffer_t* bufferC;
    php_rindow_opencl_buffer_t* bufferS;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 9, 10)
        Z_PARAM_OBJECT(a_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetA)
        Z_PARAM_OBJECT(b_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetB)
        Z_PARAM_OBJECT(c_obj_p)  // Rindow\OpenCL\Buffer

        Z_PARAM_LONG(offsetC)
        Z_PARAM_OBJECT(s_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetS)
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
    bufferS = Z_RINDOW_OPENCL_BUFFER_OBJ_P(s_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferS,"s")) {
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
    if(bufferA->dtype!=bufferB->dtype||bufferA->dtype!=bufferC->dtype||bufferA->dtype!=bufferS->dtype) {
            zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for A,B,C and S", 0);
            return;
    }

    switch (bufferA->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSrotg(
                bufferA->buffer, (size_t)offsetA,
                bufferB->buffer, (size_t)offsetB,
                bufferC->buffer, (size_t)offsetC,
                bufferS->buffer, (size_t)offsetS,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDrotg(
                bufferA->buffer, (size_t)offsetA,
                bufferB->buffer, (size_t)offsetB,
                bufferC->buffer, (size_t)offsetC,
                bufferS->buffer, (size_t)offsetS,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlast?rotg error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function rotmg(
        DeviceBuffer $D1, int $offsetD1,
        DeviceBuffer $D2, int $offsetD2,
        DeviceBuffer $B1, int $offsetB1,
        DeviceBuffer $B2, int $offsetB2,
        DeviceBuffer $P,  int $offsetP,
        CommandQueue $queue,// Rindow\OpenCL\CommandQueue
        EventList $event=null,   // Rindow\OpenCL\EventList
        ) : void
 {{{ */
static PHP_METHOD(Blas, rotmg)
{
    zval* d1_obj_p;
    zend_long offsetD1;
    zval* d2_obj_p;
    zend_long offsetD2;
    zval* b1_obj_p;
    zend_long offsetB1;
    zval* b2_obj_p;
    zend_long offsetB2;
    zval* p_obj_p;
    zend_long offsetP;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferD1;
    php_rindow_opencl_buffer_t* bufferD2;
    php_rindow_opencl_buffer_t* bufferB1;
    php_rindow_opencl_buffer_t* bufferB2;
    php_rindow_opencl_buffer_t* bufferP;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 11, 12)
        Z_PARAM_OBJECT(d1_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetD1)
        Z_PARAM_OBJECT(d2_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetD2)
        Z_PARAM_OBJECT(b1_obj_p)  // Rindow\OpenCL\Buffer

        Z_PARAM_LONG(offsetB1)
        Z_PARAM_OBJECT(b2_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetB2)
        Z_PARAM_OBJECT(p_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetP)

        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    bufferD1 = Z_RINDOW_OPENCL_BUFFER_OBJ_P(d1_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferD1,"d1")) {
        return;
    }
    bufferD2 = Z_RINDOW_OPENCL_BUFFER_OBJ_P(d2_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferD2,"d2")) {
        return;
    }
    bufferB1 = Z_RINDOW_OPENCL_BUFFER_OBJ_P(b1_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferB1,"b1")) {
        return;
    }
    bufferB2 = Z_RINDOW_OPENCL_BUFFER_OBJ_P(b2_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferB2,"b2")) {
        return;
    }
    bufferP = Z_RINDOW_OPENCL_BUFFER_OBJ_P(p_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferP,"p")) {
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
    if(bufferD1->dtype!=bufferD2->dtype||bufferD1->dtype!=bufferB1->dtype||
        bufferD1->dtype!=bufferB2->dtype||bufferD1->dtype!=bufferP->dtype) {
            zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for D1,D2,B1,B2 and P", 0);
            return;
    }

    switch (bufferD1->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSrotmg(
                bufferD1->buffer, (size_t)offsetD1,
                bufferD2->buffer, (size_t)offsetD2,
                bufferB1->buffer, (size_t)offsetB1,
                bufferB2->buffer, (size_t)offsetB2,
                bufferP->buffer,  (size_t)offsetP,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDrotmg(
                bufferD1->buffer, (size_t)offsetD1,
                bufferD2->buffer, (size_t)offsetD2,
                bufferB1->buffer, (size_t)offsetB1,
                bufferB2->buffer, (size_t)offsetB2,
                bufferP->buffer,  (size_t)offsetP,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlast?rotmg error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function rot(
        int $n,
        DeviceBuffer $X, int $offsetX, int $incX,
        DeviceBuffer $Y, int $offsetY, int $incY,
        float $cos,
        float $sin,
        CommandQueue $queue,// Rindow\OpenCL\CommandQueue
        EventList $event=null,   // Rindow\OpenCL\EventList
        ) : void
 {{{ */
static PHP_METHOD(Blas, rot)
{
    zend_long n;
    zval* x_obj_p;
    zend_long offsetX;
    zend_long incX;
    zval* y_obj_p;
    zend_long offsetY;
    zend_long incY;
    double c;
    double s;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferX;
    php_rindow_opencl_buffer_t* bufferY;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 10, 11)
        Z_PARAM_LONG(n)
        Z_PARAM_OBJECT(x_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_OBJECT(y_obj_p)  // Rindow\OpenCL\Buffer

        Z_PARAM_LONG(offsetY)
        Z_PARAM_LONG(incY)
        Z_PARAM_DOUBLE(c)
        Z_PARAM_DOUBLE(s)
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
    if(bufferX->dtype!=bufferY->dtype) {
            zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for X,Y", 0);
            return;
    }

    switch (bufferX->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSrot(
                n,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                (float)c,
                (float)s,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDrot(
                n,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                (double)c,
                (double)s,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlast?rot error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }

}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function rotm(
        int $n,
        DeviceBuffer $X, int $offsetX, int $incX,
        DeviceBuffer $Y, int $offsetY, int $incY,
        DeviceBuffer $P, int $offsetP,
        CommandQueue $queue,// Rindow\OpenCL\CommandQueue
        EventList $event=null,   // Rindow\OpenCL\EventList
        ) : void
 {{{ */
static PHP_METHOD(Blas, rotm)
{
    zend_long n;
    zval* x_obj_p;
    zend_long offsetX;
    zend_long incX;
    zval* y_obj_p;
    zend_long offsetY;
    zend_long incY;
    zval* p_obj_p;
    zend_long offsetP;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferX;
    php_rindow_opencl_buffer_t* bufferY;
    php_rindow_opencl_buffer_t* bufferP;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 10, 11)
        Z_PARAM_LONG(n)
        Z_PARAM_OBJECT(x_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_OBJECT(y_obj_p)  // Rindow\OpenCL\Buffer

        Z_PARAM_LONG(offsetY)
        Z_PARAM_LONG(incY)
        Z_PARAM_OBJECT(p_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetP)
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
    bufferP = Z_RINDOW_OPENCL_BUFFER_OBJ_P(p_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferP,"p")) {
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
    if(bufferX->dtype!=bufferY->dtype||bufferX->dtype!=bufferP->dtype) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for X,Y and P", 0);
        return;
    }

    switch (bufferX->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSrotm(
                n,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                bufferP->buffer, (size_t)offsetP,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDrotm(
                n,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                bufferP->buffer, (size_t)offsetP,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlast?rotm error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }

}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function gemv(
        int $order,
        int $trans,
        int $m,
        int $n,
        float $alpha,
        Buffer $A, int $offsetA, int $ldA,
        Buffer $X, int $offsetX, int $incX,
        float $beta,
        Buffer $Y, int $offsetY, int $incY,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Blas, gemv)
{
    zend_long order;
    zend_long trans;
    zend_long m;
    zend_long n;
    double alpha;
    zval* a_obj_p;
    zend_long offsetA;
    zend_long ldA;
    zval* x_obj_p;
    zend_long offsetX;
    zend_long incX;
    double beta;
    zval* y_obj_p;
    zend_long offsetY;
    zend_long incY;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferA;
    php_rindow_opencl_buffer_t* bufferX;
    php_rindow_opencl_buffer_t* bufferY;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 16, 17)
        Z_PARAM_LONG(order)
        Z_PARAM_LONG(trans)
        Z_PARAM_LONG(m)
        Z_PARAM_LONG(n)
        Z_PARAM_DOUBLE(alpha)
        Z_PARAM_OBJECT(a_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetA)
        Z_PARAM_LONG(ldA)
        Z_PARAM_OBJECT(x_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetX)
        Z_PARAM_LONG(incX)
        Z_PARAM_DOUBLE(beta)
        Z_PARAM_OBJECT(y_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetY)
        Z_PARAM_LONG(incY)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    bufferA = Z_RINDOW_OPENCL_BUFFER_OBJ_P(a_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferA,"a")) {
        return;
    }
    bufferX = Z_RINDOW_OPENCL_BUFFER_OBJ_P(x_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferX,"x")) {
        return;
    }
    bufferY = Z_RINDOW_OPENCL_BUFFER_OBJ_P(y_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferY,"Y")) {
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

    if(bufferA->dtype!=bufferX->dtype || bufferX->dtype!=bufferY->dtype) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for A and X and Y", 0);
        return;
    }

    switch (bufferX->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSgemv(
                (CLBlastLayout)order,
                (CLBlastTranspose)trans,
                (size_t)m,(size_t)n,
                (float)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                (float)beta,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDgemv(
                (CLBlastLayout)order,
                (CLBlastTranspose)trans,
                (size_t)m,(size_t)n,
                (double)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                bufferX->buffer, (size_t)offsetX, (size_t)incX,
                (double)beta,
                bufferY->buffer, (size_t)offsetY, (size_t)incY,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?gemv error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function gemm(
        int $order,
        int $transA,
        int $transB,
        int $m,
        int $n,
        int $k,
        float $alpha,
        Buffer $A, int $offsetA, int $ldA,
        Buffer $B, int $offsetB, int $ldB,
        float $beta,
        Buffer $C, int $offsetC, int $ldC,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Blas, gemm)
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
    zval* b_obj_p;
    zend_long offsetB;
    zend_long ldB;
    double beta;
    zval* c_obj_p;
    zend_long offsetC;
    zend_long ldC;
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

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 18, 19)
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
        Z_PARAM_OBJECT(b_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetB)
        Z_PARAM_LONG(ldB)
        Z_PARAM_DOUBLE(beta)
        Z_PARAM_OBJECT(c_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetC)
        Z_PARAM_LONG(ldC)
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
            status = CLBlastSgemm(
                (CLBlastLayout)order,
                (CLBlastTranspose)transA,
                (CLBlastTranspose)transB,
                (size_t)m, (size_t)n, (size_t)k,
                (float)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                bufferB->buffer, (size_t)offsetB, (size_t)ldB,
                (float)beta,
                bufferC->buffer, (size_t)offsetC, (size_t)ldC,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDgemm(
                (CLBlastLayout)order,
                (CLBlastTranspose)transA,
                (CLBlastTranspose)transB,
                (size_t)m, (size_t)n, (size_t)k,
                (double)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                bufferB->buffer, (size_t)offsetB, (size_t)ldB,
                (double)beta,
                bufferC->buffer, (size_t)offsetC, (size_t)ldC,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?gemm error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function symm(
        int $order,
        int $side,
        int $uplo,
        int $m,
        int $n,
        float $alpha,
        Buffer $A, int $offsetA, int $ldA,
        Buffer $B, int $offsetB, int $ldB,
        float $beta,
        Buffer $C, int $offsetC, int $ldC,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Blas, symm)
{
    zend_long order;
    zend_long side;
    zend_long uplo;
    zend_long m;
    zend_long n;
    double alpha;
    zval* a_obj_p;
    zend_long offsetA;
    zend_long ldA;
    zval* b_obj_p;
    zend_long offsetB;
    zend_long ldB;
    double beta;
    zval* c_obj_p;
    zend_long offsetC;
    zend_long ldC;
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

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 17, 18)
        Z_PARAM_LONG(order)
        Z_PARAM_LONG(side)
        Z_PARAM_LONG(uplo)
        Z_PARAM_LONG(m)
        Z_PARAM_LONG(n)
        Z_PARAM_DOUBLE(alpha)
        Z_PARAM_OBJECT(a_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetA)
        Z_PARAM_LONG(ldA)
        Z_PARAM_OBJECT(b_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetB)
        Z_PARAM_LONG(ldB)
        Z_PARAM_DOUBLE(beta)
        Z_PARAM_OBJECT(c_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetC)
        Z_PARAM_LONG(ldC)
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
            status = CLBlastSsymm(
                (CLBlastLayout)order,
                (CLBlastSide)side,
                (CLBlastTriangle)uplo,
                (size_t)m, (size_t)n,
                (float)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                bufferB->buffer, (size_t)offsetB, (size_t)ldB,
                (float)beta,
                bufferC->buffer, (size_t)offsetC, (size_t)ldC,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDsymm(
                (CLBlastLayout)order,
                (CLBlastSide)side,
                (CLBlastTriangle)uplo,
                (size_t)m, (size_t)n,
                (double)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                bufferB->buffer, (size_t)offsetB, (size_t)ldB,
                (double)beta,
                bufferC->buffer, (size_t)offsetC, (size_t)ldC,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?symm error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function syrk(
        int $order,
        int $uplo,
        int $trans,
        int $n,
        int $k,
        float $alpha,
        Buffer $A, int $offsetA, int $ldA,
        float $beta,
        Buffer $C, int $offsetC, int $ldC,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Blas, syrk)
{
    zend_long order;
    zend_long uplo;
    zend_long trans;
    zend_long n;
    zend_long k;
    double alpha;
    zval* a_obj_p;
    zend_long offsetA;
    zend_long ldA;
    double beta;
    zval* c_obj_p;
    zend_long offsetC;
    zend_long ldC;
    zval* queue_obj_p;
    zval* event_obj_p=NULL;
    php_rindow_opencl_buffer_t* bufferA;
    php_rindow_opencl_buffer_t* bufferC;
    php_rindow_opencl_command_queue_t* queue_obj;
    php_rindow_opencl_event_list_t* event_obj;
    CLBlastStatusCode status;
    cl_event event;
    cl_event *event_p=NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 14, 15)
        Z_PARAM_LONG(order)
        Z_PARAM_LONG(uplo)
        Z_PARAM_LONG(trans)
        Z_PARAM_LONG(n)
        Z_PARAM_LONG(k)
        Z_PARAM_DOUBLE(alpha)
        Z_PARAM_OBJECT(a_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetA)
        Z_PARAM_LONG(ldA)
        Z_PARAM_DOUBLE(beta)
        Z_PARAM_OBJECT(c_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetC)
        Z_PARAM_LONG(ldC)
        Z_PARAM_OBJECT(queue_obj_p)  // Rindow\OpenCL\CommandQueue
        Z_PARAM_OPTIONAL
        Z_PARAM_OBJECT_EX(event_obj_p,1,0)  // Rindow\OpenCL\EventList
    ZEND_PARSE_PARAMETERS_END();

    bufferA = Z_RINDOW_OPENCL_BUFFER_OBJ_P(a_obj_p);
    if(php_rindow_clblast_assert_opencl_buffer_type(bufferA,"a")) {
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

    if(bufferA->dtype!=bufferC->dtype) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for A and B and C", 0);
        return;
    }
    switch (bufferA->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastSsyrk(
                (CLBlastLayout)order,
                (CLBlastTriangle)uplo,
                (CLBlastTranspose)trans,
                (size_t)n, (size_t)k,
                (float)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                (float)beta,
                bufferC->buffer, (size_t)offsetC, (size_t)ldC,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDsyrk(
                (CLBlastLayout)order,
                (CLBlastTriangle)uplo,
                (CLBlastTranspose)trans,
                (size_t)n, (size_t)k,
                (double)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                (double)beta,
                bufferC->buffer, (size_t)offsetC, (size_t)ldC,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?syrk error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function syr2k(
        int $order,
        int $uplo,
        int $trans,
        int $n,
        int $k,
        float $alpha,
        Buffer $A, int $offsetA, int $ldA,
        Buffer $B, int $offsetB, int $ldB,
        float $beta,
        Buffer $C, int $offsetC, int $ldC,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Blas, syr2k)
{
    zend_long order;
    zend_long uplo;
    zend_long trans;
    zend_long n;
    zend_long k;
    double alpha;
    zval* a_obj_p;
    zend_long offsetA;
    zend_long ldA;
    zval* b_obj_p;
    zend_long offsetB;
    zend_long ldB;
    double beta;
    zval* c_obj_p;
    zend_long offsetC;
    zend_long ldC;
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

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 17, 18)
        Z_PARAM_LONG(order)
        Z_PARAM_LONG(uplo)
        Z_PARAM_LONG(trans)
        Z_PARAM_LONG(n)
        Z_PARAM_LONG(k)
        Z_PARAM_DOUBLE(alpha)
        Z_PARAM_OBJECT(a_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetA)
        Z_PARAM_LONG(ldA)
        Z_PARAM_OBJECT(b_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetB)
        Z_PARAM_LONG(ldB)
        Z_PARAM_DOUBLE(beta)
        Z_PARAM_OBJECT(c_obj_p)  // Rindow\OpenCL\Buffer
        Z_PARAM_LONG(offsetC)
        Z_PARAM_LONG(ldC)
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
            status = CLBlastSsyr2k(
                (CLBlastLayout)order,
                (CLBlastTriangle)uplo,
                (CLBlastTranspose)trans,
                (size_t)n, (size_t)k,
                (float)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                bufferB->buffer, (size_t)offsetB, (size_t)ldB,
                (float)beta,
                bufferC->buffer, (size_t)offsetC, (size_t)ldC,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDsyr2k(
                (CLBlastLayout)order,
                (CLBlastTriangle)uplo,
                (CLBlastTranspose)trans,
                (size_t)n, (size_t)k,
                (double)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                bufferB->buffer, (size_t)offsetB, (size_t)ldB,
                (double)beta,
                bufferC->buffer, (size_t)offsetC, (size_t)ldC,
                &(queue_obj->command_queue), event_p);
            break;
        default:
            zend_throw_exception(spl_ce_RuntimeException, "Unsupported data type.", 0);
            return;
    }
    if(status!=CLBlastSuccess) {
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?syr2k error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function trmm(
        int $order,
        int $side,
        int $uplo,
        int $trans,
        int $diag,
        int $m,
        int $n,
        float $alpha,
        Buffer $A, int $offsetA, int $ldA,
        Buffer $B, int $offsetB, int $ldB,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Blas, trmm)
{
    zend_long order;
    zend_long side;
    zend_long uplo;
    zend_long trans;
    zend_long diag;
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

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 15, 16)
        Z_PARAM_LONG(order)
        Z_PARAM_LONG(side)
        Z_PARAM_LONG(uplo)
        Z_PARAM_LONG(trans)
        Z_PARAM_LONG(diag)
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
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for A and B and C", 0);
        return;
    }
    switch (bufferA->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastStrmm(
                (CLBlastLayout)order,
                (CLBlastSide)side,
                (CLBlastTriangle)uplo,
                (CLBlastTranspose)trans,
                (CLBlastDiagonal)diag,
                (size_t)m, (size_t)n,
                (float)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                bufferB->buffer, (size_t)offsetB, (size_t)ldB,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDtrmm(
                (CLBlastLayout)order,
                (CLBlastSide)side,
                (CLBlastTriangle)uplo,
                (CLBlastTranspose)trans,
                (CLBlastDiagonal)diag,
                (size_t)m, (size_t)n,
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
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?trmm error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
    public function trsm(
        int $order,
        int $side,
        int $uplo,
        int $trans,
        int $diag,
        int $m,
        int $n,
        float $alpha,
        Buffer $A, int $offsetA, int $ldA,
        Buffer $B, int $offsetB, int $ldB,
        CommandQueue $queue, EventList $event
     ) : void
 {{{ */
static PHP_METHOD(Blas, trsm)
{
    zend_long order;
    zend_long side;
    zend_long uplo;
    zend_long trans;
    zend_long diag;
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

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 15, 16)
        Z_PARAM_LONG(order)
        Z_PARAM_LONG(side)
        Z_PARAM_LONG(uplo)
        Z_PARAM_LONG(trans)
        Z_PARAM_LONG(diag)
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
        zend_throw_exception(spl_ce_InvalidArgumentException, "Unmatch data type for A and B and C", 0);
        return;
    }
    switch (bufferA->dtype) {
        case php_interop_polite_math_matrix_dtype_float32:
            status = CLBlastStrsm(
                (CLBlastLayout)order,
                (CLBlastSide)side,
                (CLBlastTriangle)uplo,
                (CLBlastTranspose)trans,
                (CLBlastDiagonal)diag,
                (size_t)m, (size_t)n,
                (float)alpha,
                bufferA->buffer, (size_t)offsetA, (size_t)ldA,
                bufferB->buffer, (size_t)offsetB, (size_t)ldB,
                &(queue_obj->command_queue), event_p);
            break;
        case php_interop_polite_math_matrix_dtype_float64:
            status = CLBlastDtrsm(
                (CLBlastLayout)order,
                (CLBlastSide)side,
                (CLBlastTriangle)uplo,
                (CLBlastTranspose)trans,
                (CLBlastDiagonal)diag,
                (size_t)m, (size_t)n,
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
        zend_throw_exception_ex(spl_ce_RuntimeException, status, "CLBlasti?trsm error=%d", status);
        return;
    }

    if(php_rindow_clblast_append_event(event_obj_p, event_p)) {
        return;
    }
}
/* }}} */

/* Method Rindow\CLBlast\Blas::
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
static PHP_METHOD(Blas, omatcopy)
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


ZEND_BEGIN_ARG_INFO_EX(ai_Blas_scal, 0, 0, 6)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_INFO(0, alpha)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_axpy, 0, 0, 9)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_INFO(0, alpha)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, y, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetY)
    ZEND_ARG_INFO(0, incY)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_dot, 0, 0, 10)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, result, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetR)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, y, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetY)
    ZEND_ARG_INFO(0, incY)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_asum, 0, 0, 7)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, result, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetR)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_iamax, 0, 0, 7)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, result, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetR)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_iamin, 0, 0, 7)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, result, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetR)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_copy, 0, 0, 8)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, y, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetY)
    ZEND_ARG_INFO(0, incY)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_nrm2, 0, 0, 7)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, result, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetR)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_swap, 0, 0, 8)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, y, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetY)
    ZEND_ARG_INFO(0, incY)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_rotg, 0, 0, 9)
    ZEND_ARG_OBJ_INFO(0, a, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetA)
    ZEND_ARG_OBJ_INFO(0, b, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetB)
    ZEND_ARG_OBJ_INFO(0, c, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetC)
    ZEND_ARG_OBJ_INFO(0, s, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetS)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_rotmg, 0, 0, 11)
    ZEND_ARG_OBJ_INFO(0, d1, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetD1)
    ZEND_ARG_OBJ_INFO(0, d2, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetD2)
    ZEND_ARG_OBJ_INFO(0, b1, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetB1)
    ZEND_ARG_OBJ_INFO(0, b2, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetB2)
    ZEND_ARG_OBJ_INFO(0, p, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetP)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_rot, 0, 0, 10)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, y, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetY)
    ZEND_ARG_INFO(0, incY)
    ZEND_ARG_INFO(0, c)
    ZEND_ARG_INFO(0, s)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_rotm, 0, 0, 10)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_OBJ_INFO(0, y, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetY)
    ZEND_ARG_INFO(0, incY)
    ZEND_ARG_OBJ_INFO(0, p, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetP)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_gemv, 0, 0, 16)
    ZEND_ARG_INFO(0, order)
    ZEND_ARG_INFO(0, trans)
    ZEND_ARG_INFO(0, m)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_INFO(0, alpha)
    ZEND_ARG_OBJ_INFO(0, a, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetA)
    ZEND_ARG_INFO(0, ldA)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetX)
    ZEND_ARG_INFO(0, incX)
    ZEND_ARG_INFO(0, beta)
    ZEND_ARG_OBJ_INFO(0, x, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetY)
    ZEND_ARG_INFO(0, incY)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_gemm, 0, 0, 18)
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
    ZEND_ARG_OBJ_INFO(0, b, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetB)
    ZEND_ARG_INFO(0, ldB)
    ZEND_ARG_INFO(0, beta)
    ZEND_ARG_OBJ_INFO(0, c, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetC)
    ZEND_ARG_INFO(0, ldC)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_symm, 0, 0, 17)
    ZEND_ARG_INFO(0, order)
    ZEND_ARG_INFO(0, side)
    ZEND_ARG_INFO(0, uplo)
    ZEND_ARG_INFO(0, m)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_INFO(0, alpha)
    ZEND_ARG_OBJ_INFO(0, a, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetA)
    ZEND_ARG_INFO(0, ldA)
    ZEND_ARG_OBJ_INFO(0, b, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetB)
    ZEND_ARG_INFO(0, ldB)
    ZEND_ARG_INFO(0, beta)
    ZEND_ARG_OBJ_INFO(0, c, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetC)
    ZEND_ARG_INFO(0, ldC)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_syrk, 0, 0, 14)
    ZEND_ARG_INFO(0, order)
    ZEND_ARG_INFO(0, uplo)
    ZEND_ARG_INFO(0, trans)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_INFO(0, k)
    ZEND_ARG_INFO(0, alpha)
    ZEND_ARG_OBJ_INFO(0, a, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetA)
    ZEND_ARG_INFO(0, ldA)
    ZEND_ARG_INFO(0, beta)
    ZEND_ARG_OBJ_INFO(0, c, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetC)
    ZEND_ARG_INFO(0, ldC)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_syr2k, 0, 0, 17)
    ZEND_ARG_INFO(0, order)
    ZEND_ARG_INFO(0, uplo)
    ZEND_ARG_INFO(0, trans)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_INFO(0, k)
    ZEND_ARG_INFO(0, alpha)
    ZEND_ARG_OBJ_INFO(0, a, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetA)
    ZEND_ARG_INFO(0, ldA)
    ZEND_ARG_OBJ_INFO(0, b, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetB)
    ZEND_ARG_INFO(0, ldB)
    ZEND_ARG_INFO(0, beta)
    ZEND_ARG_OBJ_INFO(0, c, Rindow\\OpenCL\\Buffer, 0)
    ZEND_ARG_INFO(0, offsetC)
    ZEND_ARG_INFO(0, ldC)
    ZEND_ARG_OBJ_INFO(0, queue, Rindow\\OpenCL\\CommandQueue, 0)
    ZEND_ARG_OBJ_INFO(0, event, Rindow\\OpenCL\\EventList, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_trmm, 0, 0, 15)
    ZEND_ARG_INFO(0, order)
    ZEND_ARG_INFO(0, side)
    ZEND_ARG_INFO(0, uplo)
    ZEND_ARG_INFO(0, trans)
    ZEND_ARG_INFO(0, diag)
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

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_trsm, 0, 0, 15)
    ZEND_ARG_INFO(0, order)
    ZEND_ARG_INFO(0, side)
    ZEND_ARG_INFO(0, uplo)
    ZEND_ARG_INFO(0, trans)
    ZEND_ARG_INFO(0, diag)
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

ZEND_BEGIN_ARG_INFO_EX(ai_Blas_omatcopy, 0, 0, 12)
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


ZEND_BEGIN_ARG_INFO_EX(ai_Blas_void, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ Rindow\CLBlast\Blas function entries */
static zend_function_entry php_rindow_clblast_blas_me[] = {
    /* clang-format off */
    PHP_ME(Blas, scal,  ai_Blas_scal,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, axpy,  ai_Blas_axpy,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, dot,   ai_Blas_dot,   ZEND_ACC_PUBLIC)
    PHP_ME(Blas, asum,  ai_Blas_asum,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, iamax, ai_Blas_iamax, ZEND_ACC_PUBLIC)
    PHP_ME(Blas, iamin, ai_Blas_iamin, ZEND_ACC_PUBLIC)
    PHP_ME(Blas, copy,  ai_Blas_copy,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, nrm2,  ai_Blas_nrm2,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, swap,  ai_Blas_swap,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, rotg,  ai_Blas_rotg,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, rotmg, ai_Blas_rotmg, ZEND_ACC_PUBLIC)
    PHP_ME(Blas, rot,   ai_Blas_rot,   ZEND_ACC_PUBLIC)
    PHP_ME(Blas, rotm,  ai_Blas_rotm,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, gemv,  ai_Blas_gemv,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, gemm,  ai_Blas_gemm,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, symm,  ai_Blas_symm,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, syrk,  ai_Blas_syrk,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, syr2k, ai_Blas_syr2k, ZEND_ACC_PUBLIC)
    PHP_ME(Blas, trmm,  ai_Blas_trmm,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, trsm,  ai_Blas_trsm,  ZEND_ACC_PUBLIC)
    PHP_ME(Blas, omatcopy, ai_Blas_omatcopy, ZEND_ACC_PUBLIC)
    PHP_FE_END
    /* clang-format on */
};
/* }}} */

/* Class Rindow\CLBlast\Blas {{{ */
static zend_class_entry* rindow_clblast_blas_ce;

void php_rindow_clblast_blas_init_ce(INIT_FUNC_ARGS)
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "Rindow\\CLBlast", "Blas", php_rindow_clblast_blas_me);
    rindow_clblast_blas_ce = zend_register_internal_class(&ce);
    rindow_clblast_blas_ce->create_object = php_rindow_clblast_blas_create_object;

    memcpy(&rindow_clblast_blas_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    rindow_clblast_blas_object_handlers.offset    = 0;
    rindow_clblast_blas_object_handlers.free_obj  = php_rindow_clblast_blas_free_object;
    rindow_clblast_blas_object_handlers.clone_obj = NULL;

    //zend_class_implements(rindow_clblast_blas_ce, 2, spl_ce_ArrayAccess, spl_ce_Countable);
}
/* }}} */
