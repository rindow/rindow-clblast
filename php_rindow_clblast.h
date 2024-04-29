/* rindow_clblast extension for PHP */

#ifndef PHP_RINDOW_CLBLAST_H
# define PHP_RINDOW_CLBLAST_H

#include <Rindow/OpenCL/Buffer.h>
#include <Rindow/OpenCL/CommandQueue.h>
#include <Rindow/OpenCL/EventList.h>
#include <Interop/Polite/Math/Matrix.h>

# define phpext_rindow_clblast_ptr &rindow_clblast_module_entry

# define PHP_RINDOW_CLBLAST_VERSION "0.2.1"

# if defined(ZTS) && defined(COMPILE_DL_RINDOW_CLBLAST)
ZEND_TSRMLS_CACHE_EXTERN()
# endif


// Rindow\CLBlast\Blas object structures
typedef struct {
    zend_object std;
} php_rindow_clblast_blas_t;
static inline php_rindow_clblast_blas_t* php_rindow_clblast_blas_fetch_object(zend_object* obj)
{
	return (php_rindow_clblast_blas_t*) ((char*) obj - XtOffsetOf(php_rindow_clblast_blas_t, std));
}
#define Z_RINDOW_CLBLAST_BLAS_OBJ_P(zv) (php_rindow_clblast_blas_fetch_object(Z_OBJ_P(zv)))

// Rindow\CLBlast\Lapack object structures
typedef struct {
    zend_object std;
} php_rindow_clblast_lapack_t;
static inline php_rindow_clblast_lapack_t* php_rindow_clblast_lapack_fetch_object(zend_object* obj)
{
	return (php_rindow_clblast_lapack_t*) ((char*) obj - XtOffsetOf(php_rindow_clblast_lapack_t, std));
}
#define Z_RINDOW_CLBLAST_LAPACK_OBJ_P(zv) (php_rindow_clblast_lapack_fetch_object(Z_OBJ_P(zv)))

// Rindow\CLBlast\Math object structures
typedef struct {
    zend_object std;
} php_rindow_clblast_math_t;
static inline php_rindow_clblast_math_t* php_rindow_clblast_math_fetch_object(zend_object* obj)
{
	return (php_rindow_clblast_math_t*) ((char*) obj - XtOffsetOf(php_rindow_clblast_math_t, std));
}
#define Z_RINDOW_CLBLAST_MATH_OBJ_P(zv) (php_rindow_clblast_math_fetch_object(Z_OBJ_P(zv)))

extern void php_rindow_clblast_blas_init_ce(INIT_FUNC_ARGS);
extern void php_rindow_clblast_math_init_ce(INIT_FUNC_ARGS);

extern zend_module_entry rindow_clblast_module_entry;

extern int php_rindow_clblast_append_event(zval* event_list_obj_p, cl_event *event);

extern int php_rindow_clblast_assert_opencl_buffer_type(
    php_rindow_opencl_buffer_t *buffer,
    char* name);
extern int php_rindow_clblast_assert_opencl_command_queue_type(
    php_rindow_opencl_command_queue_t *queue,
    char* name);
extern int php_rindow_clblast_assert_opencl_event_list_type(
    php_rindow_opencl_event_list_t *events,
    char* name);
extern int php_rindow_clblast_assert_host_buffer_type(
    php_interop_polite_math_matrix_linear_buffer_t *buffer,
    char* name);

#endif	/* PHP_RINDOW_CLBLAST_H */
