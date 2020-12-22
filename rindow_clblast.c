/* rindow_clblast extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <php.h>
#include <Zend/zend_interfaces.h>
#include <Zend/zend_exceptions.h>
#include <ext/spl/spl_iterators.h>
#include <ext/spl/spl_exceptions.h>
#include "ext/standard/info.h"
#define CL_TARGET_OPENCL_VERSION 120
#include <CL/opencl.h>
#include <Rindow/OpenCL/EventList.h>
#include "php_rindow_clblast.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
    ZEND_PARSE_PARAMETERS_START(0, 0) \
    ZEND_PARSE_PARAMETERS_END()
#endif

int php_rindow_clblast_append_event(zval* event_list_obj_p, cl_event* event)
{
    php_rindow_opencl_event_list_t *events;
    if(event==NULL || event_list_obj_p==NULL || Z_TYPE_P(event_list_obj_p) != IS_OBJECT) {
        return 0;
    }
    events = Z_RINDOW_OPENCL_EVENT_LIST_OBJ_P(event_list_obj_p);
    events->events = erealloc(events->events, (events->num + 1)*sizeof(cl_event));
    if(events->events==NULL) {
        zend_throw_exception(spl_ce_RuntimeException, "EventList reallocate error", 0);
        return -1;
    }
    memcpy(&(events->events[events->num]),event,sizeof(cl_event));
    events->num++;
    return 0;
}

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(rindow_clblast)
{
#if defined(ZTS) && defined(COMPILE_DL_RINDOW_CLBLAST)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(rindow_clblast)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "rindow_clblast support", "enabled");
    php_info_print_table_end();
}
/* }}} */

/* {{{ rindow_clblast_functions[]
 */
//static const zend_function_entry rindow_clblast_functions[] = {
//};
/* }}} */

PHP_MINIT_FUNCTION(rindow_clblast)
{
    php_rindow_clblast_blas_init_ce(INIT_FUNC_ARGS_PASSTHRU);
    php_rindow_clblast_math_init_ce(INIT_FUNC_ARGS_PASSTHRU);
    return SUCCESS;
}

/* {{{ php_rindow_clblast_module_entry
 */
zend_module_entry rindow_clblast_module_entry = {
    STANDARD_MODULE_HEADER,
    "rindow_clblast",					/* Extension name */
    NULL,			                    /* zend_function_entry */
    PHP_MINIT(rindow_clblast),			/* PHP_MINIT - Module initialization */
    NULL,							    /* PHP_MSHUTDOWN - Module shutdown */
    PHP_RINIT(rindow_clblast),			/* PHP_RINIT - Request initialization */
    NULL,							    /* PHP_RSHUTDOWN - Request shutdown */
    PHP_MINFO(rindow_clblast),			/* PHP_MINFO - Module info */
    PHP_RINDOW_CLBLAST_VERSION,		/* Version */
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_RINDOW_CLBLAST
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(rindow_clblast)
#endif
