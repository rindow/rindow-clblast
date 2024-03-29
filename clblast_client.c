#define CL_TARGET_OPENCL_VERSION 120

#include <stdio.h>
#include <Windows.h>
#include <clblast_c.h>

#define LOADFUNC(funcname) \
_g_##funcname = (PFN##funcname)GetProcAddress( _h_clblast, #funcname ); \
if(!_g_##funcname) { \
    printf("load error: %s\n",  #funcname); \
    return -1; \
} \

static HMODULE _h_clblast = NULL;
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSrotg)( /* CLBlastSrotg */
    cl_mem            /* sa_buffer */,
    const size_t            /* sa_offset */,
    cl_mem            /* sb_buffer */,
    const size_t            /* sb_offset */,
    cl_mem            /* sc_buffer */,
    const size_t            /* sc_offset */,
    cl_mem            /* ss_buffer */,
    const size_t            /* ss_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSrotg _g_CLBlastSrotg = NULL;
CLBlastStatusCode CLBlastSrotg(
    cl_mem            sa_buffer,
    const size_t            sa_offset,
    cl_mem            sb_buffer,
    const size_t            sb_offset,
    cl_mem            sc_buffer,
    const size_t            sc_offset,
    cl_mem            ss_buffer,
    const size_t            ss_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSrotg==NULL) {
        return 0;
    }
    return _g_CLBlastSrotg(
        sa_buffer,
        sa_offset,
        sb_buffer,
        sb_offset,
        sc_buffer,
        sc_offset,
        ss_buffer,
        ss_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDrotg)( /* CLBlastDrotg */
    cl_mem            /* sa_buffer */,
    const size_t            /* sa_offset */,
    cl_mem            /* sb_buffer */,
    const size_t            /* sb_offset */,
    cl_mem            /* sc_buffer */,
    const size_t            /* sc_offset */,
    cl_mem            /* ss_buffer */,
    const size_t            /* ss_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDrotg _g_CLBlastDrotg = NULL;
CLBlastStatusCode CLBlastDrotg(
    cl_mem            sa_buffer,
    const size_t            sa_offset,
    cl_mem            sb_buffer,
    const size_t            sb_offset,
    cl_mem            sc_buffer,
    const size_t            sc_offset,
    cl_mem            ss_buffer,
    const size_t            ss_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDrotg==NULL) {
        return 0;
    }
    return _g_CLBlastDrotg(
        sa_buffer,
        sa_offset,
        sb_buffer,
        sb_offset,
        sc_buffer,
        sc_offset,
        ss_buffer,
        ss_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSrotmg)( /* CLBlastSrotmg */
    cl_mem            /* sd1_buffer */,
    const size_t            /* sd1_offset */,
    cl_mem            /* sd2_buffer */,
    const size_t            /* sd2_offset */,
    cl_mem            /* sx1_buffer */,
    const size_t            /* sx1_offset */,
    const cl_mem            /* sy1_buffer */,
    const size_t            /* sy1_offset */,
    cl_mem            /* sparam_buffer */,
    const size_t            /* sparam_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSrotmg _g_CLBlastSrotmg = NULL;
CLBlastStatusCode CLBlastSrotmg(
    cl_mem            sd1_buffer,
    const size_t            sd1_offset,
    cl_mem            sd2_buffer,
    const size_t            sd2_offset,
    cl_mem            sx1_buffer,
    const size_t            sx1_offset,
    const cl_mem            sy1_buffer,
    const size_t            sy1_offset,
    cl_mem            sparam_buffer,
    const size_t            sparam_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSrotmg==NULL) {
        return 0;
    }
    return _g_CLBlastSrotmg(
        sd1_buffer,
        sd1_offset,
        sd2_buffer,
        sd2_offset,
        sx1_buffer,
        sx1_offset,
        sy1_buffer,
        sy1_offset,
        sparam_buffer,
        sparam_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDrotmg)( /* CLBlastDrotmg */
    cl_mem            /* sd1_buffer */,
    const size_t            /* sd1_offset */,
    cl_mem            /* sd2_buffer */,
    const size_t            /* sd2_offset */,
    cl_mem            /* sx1_buffer */,
    const size_t            /* sx1_offset */,
    const cl_mem            /* sy1_buffer */,
    const size_t            /* sy1_offset */,
    cl_mem            /* sparam_buffer */,
    const size_t            /* sparam_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDrotmg _g_CLBlastDrotmg = NULL;
CLBlastStatusCode CLBlastDrotmg(
    cl_mem            sd1_buffer,
    const size_t            sd1_offset,
    cl_mem            sd2_buffer,
    const size_t            sd2_offset,
    cl_mem            sx1_buffer,
    const size_t            sx1_offset,
    const cl_mem            sy1_buffer,
    const size_t            sy1_offset,
    cl_mem            sparam_buffer,
    const size_t            sparam_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDrotmg==NULL) {
        return 0;
    }
    return _g_CLBlastDrotmg(
        sd1_buffer,
        sd1_offset,
        sd2_buffer,
        sd2_offset,
        sx1_buffer,
        sx1_offset,
        sy1_buffer,
        sy1_offset,
        sparam_buffer,
        sparam_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSrot)( /* CLBlastSrot */
    const size_t            /* n */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    const float            /* cos */,
    const float            /* sin */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSrot _g_CLBlastSrot = NULL;
CLBlastStatusCode CLBlastSrot(
    const size_t            n,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    const float            cos,
    const float            sin,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSrot==NULL) {
        return 0;
    }
    return _g_CLBlastSrot(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        cos,
        sin,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDrot)( /* CLBlastDrot */
    const size_t            /* n */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    const double            /* cos */,
    const double            /* sin */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDrot _g_CLBlastDrot = NULL;
CLBlastStatusCode CLBlastDrot(
    const size_t            n,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    const double            cos,
    const double            sin,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDrot==NULL) {
        return 0;
    }
    return _g_CLBlastDrot(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        cos,
        sin,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSrotm)( /* CLBlastSrotm */
    const size_t            /* n */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* sparam_buffer */,
    const size_t            /* sparam_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSrotm _g_CLBlastSrotm = NULL;
CLBlastStatusCode CLBlastSrotm(
    const size_t            n,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            sparam_buffer,
    const size_t            sparam_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSrotm==NULL) {
        return 0;
    }
    return _g_CLBlastSrotm(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        sparam_buffer,
        sparam_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDrotm)( /* CLBlastDrotm */
    const size_t            /* n */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* sparam_buffer */,
    const size_t            /* sparam_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDrotm _g_CLBlastDrotm = NULL;
CLBlastStatusCode CLBlastDrotm(
    const size_t            n,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            sparam_buffer,
    const size_t            sparam_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDrotm==NULL) {
        return 0;
    }
    return _g_CLBlastDrotm(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        sparam_buffer,
        sparam_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSswap)( /* CLBlastSswap */
    const size_t            /* n */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSswap _g_CLBlastSswap = NULL;
CLBlastStatusCode CLBlastSswap(
    const size_t            n,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSswap==NULL) {
        return 0;
    }
    return _g_CLBlastSswap(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDswap)( /* CLBlastDswap */
    const size_t            /* n */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDswap _g_CLBlastDswap = NULL;
CLBlastStatusCode CLBlastDswap(
    const size_t            n,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDswap==NULL) {
        return 0;
    }
    return _g_CLBlastDswap(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCswap)( /* CLBlastCswap */
    const size_t            /* n */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCswap _g_CLBlastCswap = NULL;
CLBlastStatusCode CLBlastCswap(
    const size_t            n,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCswap==NULL) {
        return 0;
    }
    return _g_CLBlastCswap(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZswap)( /* CLBlastZswap */
    const size_t            /* n */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZswap _g_CLBlastZswap = NULL;
CLBlastStatusCode CLBlastZswap(
    const size_t            n,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZswap==NULL) {
        return 0;
    }
    return _g_CLBlastZswap(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHswap)( /* CLBlastHswap */
    const size_t            /* n */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHswap _g_CLBlastHswap = NULL;
CLBlastStatusCode CLBlastHswap(
    const size_t            n,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHswap==NULL) {
        return 0;
    }
    return _g_CLBlastHswap(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSscal)( /* CLBlastSscal */
    const size_t            /* n */,
    const float            /* alpha */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSscal _g_CLBlastSscal = NULL;
CLBlastStatusCode CLBlastSscal(
    const size_t            n,
    const float            alpha,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSscal==NULL) {
        return 0;
    }
    return _g_CLBlastSscal(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDscal)( /* CLBlastDscal */
    const size_t            /* n */,
    const double            /* alpha */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDscal _g_CLBlastDscal = NULL;
CLBlastStatusCode CLBlastDscal(
    const size_t            n,
    const double            alpha,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDscal==NULL) {
        return 0;
    }
    return _g_CLBlastDscal(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCscal)( /* CLBlastCscal */
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCscal _g_CLBlastCscal = NULL;
CLBlastStatusCode CLBlastCscal(
    const size_t            n,
    const cl_float2            alpha,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCscal==NULL) {
        return 0;
    }
    return _g_CLBlastCscal(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZscal)( /* CLBlastZscal */
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZscal _g_CLBlastZscal = NULL;
CLBlastStatusCode CLBlastZscal(
    const size_t            n,
    const cl_double2            alpha,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZscal==NULL) {
        return 0;
    }
    return _g_CLBlastZscal(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHscal)( /* CLBlastHscal */
    const size_t            /* n */,
    const cl_half            /* alpha */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHscal _g_CLBlastHscal = NULL;
CLBlastStatusCode CLBlastHscal(
    const size_t            n,
    const cl_half            alpha,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHscal==NULL) {
        return 0;
    }
    return _g_CLBlastHscal(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastScopy)( /* CLBlastScopy */
    const size_t            /* n */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastScopy _g_CLBlastScopy = NULL;
CLBlastStatusCode CLBlastScopy(
    const size_t            n,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastScopy==NULL) {
        return 0;
    }
    return _g_CLBlastScopy(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDcopy)( /* CLBlastDcopy */
    const size_t            /* n */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDcopy _g_CLBlastDcopy = NULL;
CLBlastStatusCode CLBlastDcopy(
    const size_t            n,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDcopy==NULL) {
        return 0;
    }
    return _g_CLBlastDcopy(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCcopy)( /* CLBlastCcopy */
    const size_t            /* n */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCcopy _g_CLBlastCcopy = NULL;
CLBlastStatusCode CLBlastCcopy(
    const size_t            n,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCcopy==NULL) {
        return 0;
    }
    return _g_CLBlastCcopy(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZcopy)( /* CLBlastZcopy */
    const size_t            /* n */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZcopy _g_CLBlastZcopy = NULL;
CLBlastStatusCode CLBlastZcopy(
    const size_t            n,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZcopy==NULL) {
        return 0;
    }
    return _g_CLBlastZcopy(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHcopy)( /* CLBlastHcopy */
    const size_t            /* n */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHcopy _g_CLBlastHcopy = NULL;
CLBlastStatusCode CLBlastHcopy(
    const size_t            n,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHcopy==NULL) {
        return 0;
    }
    return _g_CLBlastHcopy(
        n,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSaxpy)( /* CLBlastSaxpy */
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSaxpy _g_CLBlastSaxpy = NULL;
CLBlastStatusCode CLBlastSaxpy(
    const size_t            n,
    const float            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSaxpy==NULL) {
        return 0;
    }
    return _g_CLBlastSaxpy(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDaxpy)( /* CLBlastDaxpy */
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDaxpy _g_CLBlastDaxpy = NULL;
CLBlastStatusCode CLBlastDaxpy(
    const size_t            n,
    const double            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDaxpy==NULL) {
        return 0;
    }
    return _g_CLBlastDaxpy(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCaxpy)( /* CLBlastCaxpy */
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCaxpy _g_CLBlastCaxpy = NULL;
CLBlastStatusCode CLBlastCaxpy(
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCaxpy==NULL) {
        return 0;
    }
    return _g_CLBlastCaxpy(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZaxpy)( /* CLBlastZaxpy */
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZaxpy _g_CLBlastZaxpy = NULL;
CLBlastStatusCode CLBlastZaxpy(
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZaxpy==NULL) {
        return 0;
    }
    return _g_CLBlastZaxpy(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHaxpy)( /* CLBlastHaxpy */
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHaxpy _g_CLBlastHaxpy = NULL;
CLBlastStatusCode CLBlastHaxpy(
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHaxpy==NULL) {
        return 0;
    }
    return _g_CLBlastHaxpy(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSdot)( /* CLBlastSdot */
    const size_t            /* n */,
    cl_mem            /* dot_buffer */,
    const size_t            /* dot_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSdot _g_CLBlastSdot = NULL;
CLBlastStatusCode CLBlastSdot(
    const size_t            n,
    cl_mem            dot_buffer,
    const size_t            dot_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSdot==NULL) {
        return 0;
    }
    return _g_CLBlastSdot(
        n,
        dot_buffer,
        dot_offset,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDdot)( /* CLBlastDdot */
    const size_t            /* n */,
    cl_mem            /* dot_buffer */,
    const size_t            /* dot_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDdot _g_CLBlastDdot = NULL;
CLBlastStatusCode CLBlastDdot(
    const size_t            n,
    cl_mem            dot_buffer,
    const size_t            dot_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDdot==NULL) {
        return 0;
    }
    return _g_CLBlastDdot(
        n,
        dot_buffer,
        dot_offset,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHdot)( /* CLBlastHdot */
    const size_t            /* n */,
    cl_mem            /* dot_buffer */,
    const size_t            /* dot_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHdot _g_CLBlastHdot = NULL;
CLBlastStatusCode CLBlastHdot(
    const size_t            n,
    cl_mem            dot_buffer,
    const size_t            dot_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHdot==NULL) {
        return 0;
    }
    return _g_CLBlastHdot(
        n,
        dot_buffer,
        dot_offset,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCdotu)( /* CLBlastCdotu */
    const size_t            /* n */,
    cl_mem            /* dot_buffer */,
    const size_t            /* dot_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCdotu _g_CLBlastCdotu = NULL;
CLBlastStatusCode CLBlastCdotu(
    const size_t            n,
    cl_mem            dot_buffer,
    const size_t            dot_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCdotu==NULL) {
        return 0;
    }
    return _g_CLBlastCdotu(
        n,
        dot_buffer,
        dot_offset,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZdotu)( /* CLBlastZdotu */
    const size_t            /* n */,
    cl_mem            /* dot_buffer */,
    const size_t            /* dot_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZdotu _g_CLBlastZdotu = NULL;
CLBlastStatusCode CLBlastZdotu(
    const size_t            n,
    cl_mem            dot_buffer,
    const size_t            dot_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZdotu==NULL) {
        return 0;
    }
    return _g_CLBlastZdotu(
        n,
        dot_buffer,
        dot_offset,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCdotc)( /* CLBlastCdotc */
    const size_t            /* n */,
    cl_mem            /* dot_buffer */,
    const size_t            /* dot_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCdotc _g_CLBlastCdotc = NULL;
CLBlastStatusCode CLBlastCdotc(
    const size_t            n,
    cl_mem            dot_buffer,
    const size_t            dot_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCdotc==NULL) {
        return 0;
    }
    return _g_CLBlastCdotc(
        n,
        dot_buffer,
        dot_offset,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZdotc)( /* CLBlastZdotc */
    const size_t            /* n */,
    cl_mem            /* dot_buffer */,
    const size_t            /* dot_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZdotc _g_CLBlastZdotc = NULL;
CLBlastStatusCode CLBlastZdotc(
    const size_t            n,
    cl_mem            dot_buffer,
    const size_t            dot_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZdotc==NULL) {
        return 0;
    }
    return _g_CLBlastZdotc(
        n,
        dot_buffer,
        dot_offset,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSnrm2)( /* CLBlastSnrm2 */
    const size_t            /* n */,
    cl_mem            /* nrm2_buffer */,
    const size_t            /* nrm2_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSnrm2 _g_CLBlastSnrm2 = NULL;
CLBlastStatusCode CLBlastSnrm2(
    const size_t            n,
    cl_mem            nrm2_buffer,
    const size_t            nrm2_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSnrm2==NULL) {
        return 0;
    }
    return _g_CLBlastSnrm2(
        n,
        nrm2_buffer,
        nrm2_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDnrm2)( /* CLBlastDnrm2 */
    const size_t            /* n */,
    cl_mem            /* nrm2_buffer */,
    const size_t            /* nrm2_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDnrm2 _g_CLBlastDnrm2 = NULL;
CLBlastStatusCode CLBlastDnrm2(
    const size_t            n,
    cl_mem            nrm2_buffer,
    const size_t            nrm2_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDnrm2==NULL) {
        return 0;
    }
    return _g_CLBlastDnrm2(
        n,
        nrm2_buffer,
        nrm2_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastScnrm2)( /* CLBlastScnrm2 */
    const size_t            /* n */,
    cl_mem            /* nrm2_buffer */,
    const size_t            /* nrm2_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastScnrm2 _g_CLBlastScnrm2 = NULL;
CLBlastStatusCode CLBlastScnrm2(
    const size_t            n,
    cl_mem            nrm2_buffer,
    const size_t            nrm2_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastScnrm2==NULL) {
        return 0;
    }
    return _g_CLBlastScnrm2(
        n,
        nrm2_buffer,
        nrm2_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDznrm2)( /* CLBlastDznrm2 */
    const size_t            /* n */,
    cl_mem            /* nrm2_buffer */,
    const size_t            /* nrm2_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDznrm2 _g_CLBlastDznrm2 = NULL;
CLBlastStatusCode CLBlastDznrm2(
    const size_t            n,
    cl_mem            nrm2_buffer,
    const size_t            nrm2_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDznrm2==NULL) {
        return 0;
    }
    return _g_CLBlastDznrm2(
        n,
        nrm2_buffer,
        nrm2_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHnrm2)( /* CLBlastHnrm2 */
    const size_t            /* n */,
    cl_mem            /* nrm2_buffer */,
    const size_t            /* nrm2_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHnrm2 _g_CLBlastHnrm2 = NULL;
CLBlastStatusCode CLBlastHnrm2(
    const size_t            n,
    cl_mem            nrm2_buffer,
    const size_t            nrm2_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHnrm2==NULL) {
        return 0;
    }
    return _g_CLBlastHnrm2(
        n,
        nrm2_buffer,
        nrm2_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSasum)( /* CLBlastSasum */
    const size_t            /* n */,
    cl_mem            /* asum_buffer */,
    const size_t            /* asum_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSasum _g_CLBlastSasum = NULL;
CLBlastStatusCode CLBlastSasum(
    const size_t            n,
    cl_mem            asum_buffer,
    const size_t            asum_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSasum==NULL) {
        return 0;
    }
    return _g_CLBlastSasum(
        n,
        asum_buffer,
        asum_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDasum)( /* CLBlastDasum */
    const size_t            /* n */,
    cl_mem            /* asum_buffer */,
    const size_t            /* asum_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDasum _g_CLBlastDasum = NULL;
CLBlastStatusCode CLBlastDasum(
    const size_t            n,
    cl_mem            asum_buffer,
    const size_t            asum_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDasum==NULL) {
        return 0;
    }
    return _g_CLBlastDasum(
        n,
        asum_buffer,
        asum_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastScasum)( /* CLBlastScasum */
    const size_t            /* n */,
    cl_mem            /* asum_buffer */,
    const size_t            /* asum_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastScasum _g_CLBlastScasum = NULL;
CLBlastStatusCode CLBlastScasum(
    const size_t            n,
    cl_mem            asum_buffer,
    const size_t            asum_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastScasum==NULL) {
        return 0;
    }
    return _g_CLBlastScasum(
        n,
        asum_buffer,
        asum_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDzasum)( /* CLBlastDzasum */
    const size_t            /* n */,
    cl_mem            /* asum_buffer */,
    const size_t            /* asum_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDzasum _g_CLBlastDzasum = NULL;
CLBlastStatusCode CLBlastDzasum(
    const size_t            n,
    cl_mem            asum_buffer,
    const size_t            asum_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDzasum==NULL) {
        return 0;
    }
    return _g_CLBlastDzasum(
        n,
        asum_buffer,
        asum_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHasum)( /* CLBlastHasum */
    const size_t            /* n */,
    cl_mem            /* asum_buffer */,
    const size_t            /* asum_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHasum _g_CLBlastHasum = NULL;
CLBlastStatusCode CLBlastHasum(
    const size_t            n,
    cl_mem            asum_buffer,
    const size_t            asum_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHasum==NULL) {
        return 0;
    }
    return _g_CLBlastHasum(
        n,
        asum_buffer,
        asum_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSsum)( /* CLBlastSsum */
    const size_t            /* n */,
    cl_mem            /* sum_buffer */,
    const size_t            /* sum_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSsum _g_CLBlastSsum = NULL;
CLBlastStatusCode CLBlastSsum(
    const size_t            n,
    cl_mem            sum_buffer,
    const size_t            sum_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSsum==NULL) {
        return 0;
    }
    return _g_CLBlastSsum(
        n,
        sum_buffer,
        sum_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDsum)( /* CLBlastDsum */
    const size_t            /* n */,
    cl_mem            /* sum_buffer */,
    const size_t            /* sum_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDsum _g_CLBlastDsum = NULL;
CLBlastStatusCode CLBlastDsum(
    const size_t            n,
    cl_mem            sum_buffer,
    const size_t            sum_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDsum==NULL) {
        return 0;
    }
    return _g_CLBlastDsum(
        n,
        sum_buffer,
        sum_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastScsum)( /* CLBlastScsum */
    const size_t            /* n */,
    cl_mem            /* sum_buffer */,
    const size_t            /* sum_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastScsum _g_CLBlastScsum = NULL;
CLBlastStatusCode CLBlastScsum(
    const size_t            n,
    cl_mem            sum_buffer,
    const size_t            sum_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastScsum==NULL) {
        return 0;
    }
    return _g_CLBlastScsum(
        n,
        sum_buffer,
        sum_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDzsum)( /* CLBlastDzsum */
    const size_t            /* n */,
    cl_mem            /* sum_buffer */,
    const size_t            /* sum_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDzsum _g_CLBlastDzsum = NULL;
CLBlastStatusCode CLBlastDzsum(
    const size_t            n,
    cl_mem            sum_buffer,
    const size_t            sum_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDzsum==NULL) {
        return 0;
    }
    return _g_CLBlastDzsum(
        n,
        sum_buffer,
        sum_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHsum)( /* CLBlastHsum */
    const size_t            /* n */,
    cl_mem            /* sum_buffer */,
    const size_t            /* sum_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHsum _g_CLBlastHsum = NULL;
CLBlastStatusCode CLBlastHsum(
    const size_t            n,
    cl_mem            sum_buffer,
    const size_t            sum_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHsum==NULL) {
        return 0;
    }
    return _g_CLBlastHsum(
        n,
        sum_buffer,
        sum_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiSamax)( /* CLBlastiSamax */
    const size_t            /* n */,
    cl_mem            /* imax_buffer */,
    const size_t            /* imax_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiSamax _g_CLBlastiSamax = NULL;
CLBlastStatusCode CLBlastiSamax(
    const size_t            n,
    cl_mem            imax_buffer,
    const size_t            imax_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiSamax==NULL) {
        return 0;
    }
    return _g_CLBlastiSamax(
        n,
        imax_buffer,
        imax_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiDamax)( /* CLBlastiDamax */
    const size_t            /* n */,
    cl_mem            /* imax_buffer */,
    const size_t            /* imax_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiDamax _g_CLBlastiDamax = NULL;
CLBlastStatusCode CLBlastiDamax(
    const size_t            n,
    cl_mem            imax_buffer,
    const size_t            imax_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiDamax==NULL) {
        return 0;
    }
    return _g_CLBlastiDamax(
        n,
        imax_buffer,
        imax_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiCamax)( /* CLBlastiCamax */
    const size_t            /* n */,
    cl_mem            /* imax_buffer */,
    const size_t            /* imax_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiCamax _g_CLBlastiCamax = NULL;
CLBlastStatusCode CLBlastiCamax(
    const size_t            n,
    cl_mem            imax_buffer,
    const size_t            imax_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiCamax==NULL) {
        return 0;
    }
    return _g_CLBlastiCamax(
        n,
        imax_buffer,
        imax_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiZamax)( /* CLBlastiZamax */
    const size_t            /* n */,
    cl_mem            /* imax_buffer */,
    const size_t            /* imax_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiZamax _g_CLBlastiZamax = NULL;
CLBlastStatusCode CLBlastiZamax(
    const size_t            n,
    cl_mem            imax_buffer,
    const size_t            imax_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiZamax==NULL) {
        return 0;
    }
    return _g_CLBlastiZamax(
        n,
        imax_buffer,
        imax_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiHamax)( /* CLBlastiHamax */
    const size_t            /* n */,
    cl_mem            /* imax_buffer */,
    const size_t            /* imax_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiHamax _g_CLBlastiHamax = NULL;
CLBlastStatusCode CLBlastiHamax(
    const size_t            n,
    cl_mem            imax_buffer,
    const size_t            imax_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiHamax==NULL) {
        return 0;
    }
    return _g_CLBlastiHamax(
        n,
        imax_buffer,
        imax_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiSamin)( /* CLBlastiSamin */
    const size_t            /* n */,
    cl_mem            /* imin_buffer */,
    const size_t            /* imin_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiSamin _g_CLBlastiSamin = NULL;
CLBlastStatusCode CLBlastiSamin(
    const size_t            n,
    cl_mem            imin_buffer,
    const size_t            imin_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiSamin==NULL) {
        return 0;
    }
    return _g_CLBlastiSamin(
        n,
        imin_buffer,
        imin_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiDamin)( /* CLBlastiDamin */
    const size_t            /* n */,
    cl_mem            /* imin_buffer */,
    const size_t            /* imin_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiDamin _g_CLBlastiDamin = NULL;
CLBlastStatusCode CLBlastiDamin(
    const size_t            n,
    cl_mem            imin_buffer,
    const size_t            imin_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiDamin==NULL) {
        return 0;
    }
    return _g_CLBlastiDamin(
        n,
        imin_buffer,
        imin_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiCamin)( /* CLBlastiCamin */
    const size_t            /* n */,
    cl_mem            /* imin_buffer */,
    const size_t            /* imin_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiCamin _g_CLBlastiCamin = NULL;
CLBlastStatusCode CLBlastiCamin(
    const size_t            n,
    cl_mem            imin_buffer,
    const size_t            imin_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiCamin==NULL) {
        return 0;
    }
    return _g_CLBlastiCamin(
        n,
        imin_buffer,
        imin_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiZamin)( /* CLBlastiZamin */
    const size_t            /* n */,
    cl_mem            /* imin_buffer */,
    const size_t            /* imin_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiZamin _g_CLBlastiZamin = NULL;
CLBlastStatusCode CLBlastiZamin(
    const size_t            n,
    cl_mem            imin_buffer,
    const size_t            imin_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiZamin==NULL) {
        return 0;
    }
    return _g_CLBlastiZamin(
        n,
        imin_buffer,
        imin_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiHamin)( /* CLBlastiHamin */
    const size_t            /* n */,
    cl_mem            /* imin_buffer */,
    const size_t            /* imin_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiHamin _g_CLBlastiHamin = NULL;
CLBlastStatusCode CLBlastiHamin(
    const size_t            n,
    cl_mem            imin_buffer,
    const size_t            imin_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiHamin==NULL) {
        return 0;
    }
    return _g_CLBlastiHamin(
        n,
        imin_buffer,
        imin_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiSmax)( /* CLBlastiSmax */
    const size_t            /* n */,
    cl_mem            /* imax_buffer */,
    const size_t            /* imax_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiSmax _g_CLBlastiSmax = NULL;
CLBlastStatusCode CLBlastiSmax(
    const size_t            n,
    cl_mem            imax_buffer,
    const size_t            imax_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiSmax==NULL) {
        return 0;
    }
    return _g_CLBlastiSmax(
        n,
        imax_buffer,
        imax_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiDmax)( /* CLBlastiDmax */
    const size_t            /* n */,
    cl_mem            /* imax_buffer */,
    const size_t            /* imax_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiDmax _g_CLBlastiDmax = NULL;
CLBlastStatusCode CLBlastiDmax(
    const size_t            n,
    cl_mem            imax_buffer,
    const size_t            imax_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiDmax==NULL) {
        return 0;
    }
    return _g_CLBlastiDmax(
        n,
        imax_buffer,
        imax_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiCmax)( /* CLBlastiCmax */
    const size_t            /* n */,
    cl_mem            /* imax_buffer */,
    const size_t            /* imax_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiCmax _g_CLBlastiCmax = NULL;
CLBlastStatusCode CLBlastiCmax(
    const size_t            n,
    cl_mem            imax_buffer,
    const size_t            imax_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiCmax==NULL) {
        return 0;
    }
    return _g_CLBlastiCmax(
        n,
        imax_buffer,
        imax_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiZmax)( /* CLBlastiZmax */
    const size_t            /* n */,
    cl_mem            /* imax_buffer */,
    const size_t            /* imax_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiZmax _g_CLBlastiZmax = NULL;
CLBlastStatusCode CLBlastiZmax(
    const size_t            n,
    cl_mem            imax_buffer,
    const size_t            imax_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiZmax==NULL) {
        return 0;
    }
    return _g_CLBlastiZmax(
        n,
        imax_buffer,
        imax_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiHmax)( /* CLBlastiHmax */
    const size_t            /* n */,
    cl_mem            /* imax_buffer */,
    const size_t            /* imax_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiHmax _g_CLBlastiHmax = NULL;
CLBlastStatusCode CLBlastiHmax(
    const size_t            n,
    cl_mem            imax_buffer,
    const size_t            imax_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiHmax==NULL) {
        return 0;
    }
    return _g_CLBlastiHmax(
        n,
        imax_buffer,
        imax_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiSmin)( /* CLBlastiSmin */
    const size_t            /* n */,
    cl_mem            /* imin_buffer */,
    const size_t            /* imin_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiSmin _g_CLBlastiSmin = NULL;
CLBlastStatusCode CLBlastiSmin(
    const size_t            n,
    cl_mem            imin_buffer,
    const size_t            imin_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiSmin==NULL) {
        return 0;
    }
    return _g_CLBlastiSmin(
        n,
        imin_buffer,
        imin_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiDmin)( /* CLBlastiDmin */
    const size_t            /* n */,
    cl_mem            /* imin_buffer */,
    const size_t            /* imin_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiDmin _g_CLBlastiDmin = NULL;
CLBlastStatusCode CLBlastiDmin(
    const size_t            n,
    cl_mem            imin_buffer,
    const size_t            imin_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiDmin==NULL) {
        return 0;
    }
    return _g_CLBlastiDmin(
        n,
        imin_buffer,
        imin_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiCmin)( /* CLBlastiCmin */
    const size_t            /* n */,
    cl_mem            /* imin_buffer */,
    const size_t            /* imin_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiCmin _g_CLBlastiCmin = NULL;
CLBlastStatusCode CLBlastiCmin(
    const size_t            n,
    cl_mem            imin_buffer,
    const size_t            imin_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiCmin==NULL) {
        return 0;
    }
    return _g_CLBlastiCmin(
        n,
        imin_buffer,
        imin_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiZmin)( /* CLBlastiZmin */
    const size_t            /* n */,
    cl_mem            /* imin_buffer */,
    const size_t            /* imin_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiZmin _g_CLBlastiZmin = NULL;
CLBlastStatusCode CLBlastiZmin(
    const size_t            n,
    cl_mem            imin_buffer,
    const size_t            imin_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiZmin==NULL) {
        return 0;
    }
    return _g_CLBlastiZmin(
        n,
        imin_buffer,
        imin_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastiHmin)( /* CLBlastiHmin */
    const size_t            /* n */,
    cl_mem            /* imin_buffer */,
    const size_t            /* imin_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastiHmin _g_CLBlastiHmin = NULL;
CLBlastStatusCode CLBlastiHmin(
    const size_t            n,
    cl_mem            imin_buffer,
    const size_t            imin_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastiHmin==NULL) {
        return 0;
    }
    return _g_CLBlastiHmin(
        n,
        imin_buffer,
        imin_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSgemv)( /* CLBlastSgemv */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const float            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSgemv _g_CLBlastSgemv = NULL;
CLBlastStatusCode CLBlastSgemv(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const float            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSgemv==NULL) {
        return 0;
    }
    return _g_CLBlastSgemv(
        layout,
        a_transpose,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDgemv)( /* CLBlastDgemv */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const double            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDgemv _g_CLBlastDgemv = NULL;
CLBlastStatusCode CLBlastDgemv(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const double            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDgemv==NULL) {
        return 0;
    }
    return _g_CLBlastDgemv(
        layout,
        a_transpose,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCgemv)( /* CLBlastCgemv */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_float2            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCgemv _g_CLBlastCgemv = NULL;
CLBlastStatusCode CLBlastCgemv(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_float2            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCgemv==NULL) {
        return 0;
    }
    return _g_CLBlastCgemv(
        layout,
        a_transpose,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZgemv)( /* CLBlastZgemv */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_double2            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZgemv _g_CLBlastZgemv = NULL;
CLBlastStatusCode CLBlastZgemv(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_double2            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZgemv==NULL) {
        return 0;
    }
    return _g_CLBlastZgemv(
        layout,
        a_transpose,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHgemv)( /* CLBlastHgemv */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_half            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHgemv _g_CLBlastHgemv = NULL;
CLBlastStatusCode CLBlastHgemv(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_half            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHgemv==NULL) {
        return 0;
    }
    return _g_CLBlastHgemv(
        layout,
        a_transpose,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSgbmv)( /* CLBlastSgbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* kl */,
    const size_t            /* ku */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const float            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSgbmv _g_CLBlastSgbmv = NULL;
CLBlastStatusCode CLBlastSgbmv(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            kl,
    const size_t            ku,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const float            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSgbmv==NULL) {
        return 0;
    }
    return _g_CLBlastSgbmv(
        layout,
        a_transpose,
        m,
        n,
        kl,
        ku,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDgbmv)( /* CLBlastDgbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* kl */,
    const size_t            /* ku */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const double            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDgbmv _g_CLBlastDgbmv = NULL;
CLBlastStatusCode CLBlastDgbmv(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            kl,
    const size_t            ku,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const double            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDgbmv==NULL) {
        return 0;
    }
    return _g_CLBlastDgbmv(
        layout,
        a_transpose,
        m,
        n,
        kl,
        ku,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCgbmv)( /* CLBlastCgbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* kl */,
    const size_t            /* ku */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_float2            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCgbmv _g_CLBlastCgbmv = NULL;
CLBlastStatusCode CLBlastCgbmv(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            kl,
    const size_t            ku,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_float2            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCgbmv==NULL) {
        return 0;
    }
    return _g_CLBlastCgbmv(
        layout,
        a_transpose,
        m,
        n,
        kl,
        ku,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZgbmv)( /* CLBlastZgbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* kl */,
    const size_t            /* ku */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_double2            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZgbmv _g_CLBlastZgbmv = NULL;
CLBlastStatusCode CLBlastZgbmv(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            kl,
    const size_t            ku,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_double2            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZgbmv==NULL) {
        return 0;
    }
    return _g_CLBlastZgbmv(
        layout,
        a_transpose,
        m,
        n,
        kl,
        ku,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHgbmv)( /* CLBlastHgbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* kl */,
    const size_t            /* ku */,
    const cl_half            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_half            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHgbmv _g_CLBlastHgbmv = NULL;
CLBlastStatusCode CLBlastHgbmv(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            kl,
    const size_t            ku,
    const cl_half            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_half            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHgbmv==NULL) {
        return 0;
    }
    return _g_CLBlastHgbmv(
        layout,
        a_transpose,
        m,
        n,
        kl,
        ku,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastChemv)( /* CLBlastChemv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_float2            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastChemv _g_CLBlastChemv = NULL;
CLBlastStatusCode CLBlastChemv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_float2            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastChemv==NULL) {
        return 0;
    }
    return _g_CLBlastChemv(
        layout,
        triangle,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZhemv)( /* CLBlastZhemv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_double2            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZhemv _g_CLBlastZhemv = NULL;
CLBlastStatusCode CLBlastZhemv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_double2            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZhemv==NULL) {
        return 0;
    }
    return _g_CLBlastZhemv(
        layout,
        triangle,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastChbmv)( /* CLBlastChbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_float2            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastChbmv _g_CLBlastChbmv = NULL;
CLBlastStatusCode CLBlastChbmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const size_t            k,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_float2            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastChbmv==NULL) {
        return 0;
    }
    return _g_CLBlastChbmv(
        layout,
        triangle,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZhbmv)( /* CLBlastZhbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_double2            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZhbmv _g_CLBlastZhbmv = NULL;
CLBlastStatusCode CLBlastZhbmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const size_t            k,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_double2            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZhbmv==NULL) {
        return 0;
    }
    return _g_CLBlastZhbmv(
        layout,
        triangle,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastChpmv)( /* CLBlastChpmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_float2            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastChpmv _g_CLBlastChpmv = NULL;
CLBlastStatusCode CLBlastChpmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_float2            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastChpmv==NULL) {
        return 0;
    }
    return _g_CLBlastChpmv(
        layout,
        triangle,
        n,
        alpha,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZhpmv)( /* CLBlastZhpmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_double2            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZhpmv _g_CLBlastZhpmv = NULL;
CLBlastStatusCode CLBlastZhpmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_double2            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZhpmv==NULL) {
        return 0;
    }
    return _g_CLBlastZhpmv(
        layout,
        triangle,
        n,
        alpha,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSsymv)( /* CLBlastSsymv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const float            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSsymv _g_CLBlastSsymv = NULL;
CLBlastStatusCode CLBlastSsymv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const float            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSsymv==NULL) {
        return 0;
    }
    return _g_CLBlastSsymv(
        layout,
        triangle,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDsymv)( /* CLBlastDsymv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const double            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDsymv _g_CLBlastDsymv = NULL;
CLBlastStatusCode CLBlastDsymv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const double            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDsymv==NULL) {
        return 0;
    }
    return _g_CLBlastDsymv(
        layout,
        triangle,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHsymv)( /* CLBlastHsymv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_half            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHsymv _g_CLBlastHsymv = NULL;
CLBlastStatusCode CLBlastHsymv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_half            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHsymv==NULL) {
        return 0;
    }
    return _g_CLBlastHsymv(
        layout,
        triangle,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSsbmv)( /* CLBlastSsbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const size_t            /* k */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const float            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSsbmv _g_CLBlastSsbmv = NULL;
CLBlastStatusCode CLBlastSsbmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const size_t            k,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const float            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSsbmv==NULL) {
        return 0;
    }
    return _g_CLBlastSsbmv(
        layout,
        triangle,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDsbmv)( /* CLBlastDsbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const size_t            /* k */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const double            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDsbmv _g_CLBlastDsbmv = NULL;
CLBlastStatusCode CLBlastDsbmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const size_t            k,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const double            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDsbmv==NULL) {
        return 0;
    }
    return _g_CLBlastDsbmv(
        layout,
        triangle,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHsbmv)( /* CLBlastHsbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_half            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_half            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHsbmv _g_CLBlastHsbmv = NULL;
CLBlastStatusCode CLBlastHsbmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const size_t            k,
    const cl_half            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_half            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHsbmv==NULL) {
        return 0;
    }
    return _g_CLBlastHsbmv(
        layout,
        triangle,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSspmv)( /* CLBlastSspmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const float            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSspmv _g_CLBlastSspmv = NULL;
CLBlastStatusCode CLBlastSspmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const float            alpha,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const float            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSspmv==NULL) {
        return 0;
    }
    return _g_CLBlastSspmv(
        layout,
        triangle,
        n,
        alpha,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDspmv)( /* CLBlastDspmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const double            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDspmv _g_CLBlastDspmv = NULL;
CLBlastStatusCode CLBlastDspmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const double            alpha,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const double            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDspmv==NULL) {
        return 0;
    }
    return _g_CLBlastDspmv(
        layout,
        triangle,
        n,
        alpha,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHspmv)( /* CLBlastHspmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_half            /* beta */,
    cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHspmv _g_CLBlastHspmv = NULL;
CLBlastStatusCode CLBlastHspmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_half            beta,
    cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHspmv==NULL) {
        return 0;
    }
    return _g_CLBlastHspmv(
        layout,
        triangle,
        n,
        alpha,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        beta,
        y_buffer,
        y_offset,
        y_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastStrmv)( /* CLBlastStrmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastStrmv _g_CLBlastStrmv = NULL;
CLBlastStatusCode CLBlastStrmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastStrmv==NULL) {
        return 0;
    }
    return _g_CLBlastStrmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDtrmv)( /* CLBlastDtrmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDtrmv _g_CLBlastDtrmv = NULL;
CLBlastStatusCode CLBlastDtrmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDtrmv==NULL) {
        return 0;
    }
    return _g_CLBlastDtrmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCtrmv)( /* CLBlastCtrmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCtrmv _g_CLBlastCtrmv = NULL;
CLBlastStatusCode CLBlastCtrmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCtrmv==NULL) {
        return 0;
    }
    return _g_CLBlastCtrmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZtrmv)( /* CLBlastZtrmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZtrmv _g_CLBlastZtrmv = NULL;
CLBlastStatusCode CLBlastZtrmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZtrmv==NULL) {
        return 0;
    }
    return _g_CLBlastZtrmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHtrmv)( /* CLBlastHtrmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHtrmv _g_CLBlastHtrmv = NULL;
CLBlastStatusCode CLBlastHtrmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHtrmv==NULL) {
        return 0;
    }
    return _g_CLBlastHtrmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastStbmv)( /* CLBlastStbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastStbmv _g_CLBlastStbmv = NULL;
CLBlastStatusCode CLBlastStbmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const size_t            k,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastStbmv==NULL) {
        return 0;
    }
    return _g_CLBlastStbmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        k,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDtbmv)( /* CLBlastDtbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDtbmv _g_CLBlastDtbmv = NULL;
CLBlastStatusCode CLBlastDtbmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const size_t            k,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDtbmv==NULL) {
        return 0;
    }
    return _g_CLBlastDtbmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        k,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCtbmv)( /* CLBlastCtbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCtbmv _g_CLBlastCtbmv = NULL;
CLBlastStatusCode CLBlastCtbmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const size_t            k,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCtbmv==NULL) {
        return 0;
    }
    return _g_CLBlastCtbmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        k,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZtbmv)( /* CLBlastZtbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZtbmv _g_CLBlastZtbmv = NULL;
CLBlastStatusCode CLBlastZtbmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const size_t            k,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZtbmv==NULL) {
        return 0;
    }
    return _g_CLBlastZtbmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        k,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHtbmv)( /* CLBlastHtbmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHtbmv _g_CLBlastHtbmv = NULL;
CLBlastStatusCode CLBlastHtbmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const size_t            k,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHtbmv==NULL) {
        return 0;
    }
    return _g_CLBlastHtbmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        k,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastStpmv)( /* CLBlastStpmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastStpmv _g_CLBlastStpmv = NULL;
CLBlastStatusCode CLBlastStpmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastStpmv==NULL) {
        return 0;
    }
    return _g_CLBlastStpmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDtpmv)( /* CLBlastDtpmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDtpmv _g_CLBlastDtpmv = NULL;
CLBlastStatusCode CLBlastDtpmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDtpmv==NULL) {
        return 0;
    }
    return _g_CLBlastDtpmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCtpmv)( /* CLBlastCtpmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCtpmv _g_CLBlastCtpmv = NULL;
CLBlastStatusCode CLBlastCtpmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCtpmv==NULL) {
        return 0;
    }
    return _g_CLBlastCtpmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZtpmv)( /* CLBlastZtpmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZtpmv _g_CLBlastZtpmv = NULL;
CLBlastStatusCode CLBlastZtpmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZtpmv==NULL) {
        return 0;
    }
    return _g_CLBlastZtpmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHtpmv)( /* CLBlastHtpmv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHtpmv _g_CLBlastHtpmv = NULL;
CLBlastStatusCode CLBlastHtpmv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHtpmv==NULL) {
        return 0;
    }
    return _g_CLBlastHtpmv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastStrsv)( /* CLBlastStrsv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastStrsv _g_CLBlastStrsv = NULL;
CLBlastStatusCode CLBlastStrsv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastStrsv==NULL) {
        return 0;
    }
    return _g_CLBlastStrsv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDtrsv)( /* CLBlastDtrsv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDtrsv _g_CLBlastDtrsv = NULL;
CLBlastStatusCode CLBlastDtrsv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDtrsv==NULL) {
        return 0;
    }
    return _g_CLBlastDtrsv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCtrsv)( /* CLBlastCtrsv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCtrsv _g_CLBlastCtrsv = NULL;
CLBlastStatusCode CLBlastCtrsv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCtrsv==NULL) {
        return 0;
    }
    return _g_CLBlastCtrsv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZtrsv)( /* CLBlastZtrsv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZtrsv _g_CLBlastZtrsv = NULL;
CLBlastStatusCode CLBlastZtrsv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZtrsv==NULL) {
        return 0;
    }
    return _g_CLBlastZtrsv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastStbsv)( /* CLBlastStbsv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastStbsv _g_CLBlastStbsv = NULL;
CLBlastStatusCode CLBlastStbsv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const size_t            k,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastStbsv==NULL) {
        return 0;
    }
    return _g_CLBlastStbsv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        k,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDtbsv)( /* CLBlastDtbsv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDtbsv _g_CLBlastDtbsv = NULL;
CLBlastStatusCode CLBlastDtbsv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const size_t            k,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDtbsv==NULL) {
        return 0;
    }
    return _g_CLBlastDtbsv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        k,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCtbsv)( /* CLBlastCtbsv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCtbsv _g_CLBlastCtbsv = NULL;
CLBlastStatusCode CLBlastCtbsv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const size_t            k,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCtbsv==NULL) {
        return 0;
    }
    return _g_CLBlastCtbsv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        k,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZtbsv)( /* CLBlastZtbsv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZtbsv _g_CLBlastZtbsv = NULL;
CLBlastStatusCode CLBlastZtbsv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const size_t            k,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZtbsv==NULL) {
        return 0;
    }
    return _g_CLBlastZtbsv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        k,
        a_buffer,
        a_offset,
        a_ld,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastStpsv)( /* CLBlastStpsv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastStpsv _g_CLBlastStpsv = NULL;
CLBlastStatusCode CLBlastStpsv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastStpsv==NULL) {
        return 0;
    }
    return _g_CLBlastStpsv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDtpsv)( /* CLBlastDtpsv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDtpsv _g_CLBlastDtpsv = NULL;
CLBlastStatusCode CLBlastDtpsv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDtpsv==NULL) {
        return 0;
    }
    return _g_CLBlastDtpsv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCtpsv)( /* CLBlastCtpsv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCtpsv _g_CLBlastCtpsv = NULL;
CLBlastStatusCode CLBlastCtpsv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCtpsv==NULL) {
        return 0;
    }
    return _g_CLBlastCtpsv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZtpsv)( /* CLBlastZtpsv */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* n */,
    const cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZtpsv _g_CLBlastZtpsv = NULL;
CLBlastStatusCode CLBlastZtpsv(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            n,
    const cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZtpsv==NULL) {
        return 0;
    }
    return _g_CLBlastZtpsv(
        layout,
        triangle,
        a_transpose,
        diagonal,
        n,
        ap_buffer,
        ap_offset,
        x_buffer,
        x_offset,
        x_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSger)( /* CLBlastSger */
    const CLBlastLayout            /* layout */,
    const size_t            /* m */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSger _g_CLBlastSger = NULL;
CLBlastStatusCode CLBlastSger(
    const CLBlastLayout            layout,
    const size_t            m,
    const size_t            n,
    const float            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSger==NULL) {
        return 0;
    }
    return _g_CLBlastSger(
        layout,
        m,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDger)( /* CLBlastDger */
    const CLBlastLayout            /* layout */,
    const size_t            /* m */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDger _g_CLBlastDger = NULL;
CLBlastStatusCode CLBlastDger(
    const CLBlastLayout            layout,
    const size_t            m,
    const size_t            n,
    const double            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDger==NULL) {
        return 0;
    }
    return _g_CLBlastDger(
        layout,
        m,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHger)( /* CLBlastHger */
    const CLBlastLayout            /* layout */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHger _g_CLBlastHger = NULL;
CLBlastStatusCode CLBlastHger(
    const CLBlastLayout            layout,
    const size_t            m,
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHger==NULL) {
        return 0;
    }
    return _g_CLBlastHger(
        layout,
        m,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCgeru)( /* CLBlastCgeru */
    const CLBlastLayout            /* layout */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCgeru _g_CLBlastCgeru = NULL;
CLBlastStatusCode CLBlastCgeru(
    const CLBlastLayout            layout,
    const size_t            m,
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCgeru==NULL) {
        return 0;
    }
    return _g_CLBlastCgeru(
        layout,
        m,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZgeru)( /* CLBlastZgeru */
    const CLBlastLayout            /* layout */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZgeru _g_CLBlastZgeru = NULL;
CLBlastStatusCode CLBlastZgeru(
    const CLBlastLayout            layout,
    const size_t            m,
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZgeru==NULL) {
        return 0;
    }
    return _g_CLBlastZgeru(
        layout,
        m,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCgerc)( /* CLBlastCgerc */
    const CLBlastLayout            /* layout */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCgerc _g_CLBlastCgerc = NULL;
CLBlastStatusCode CLBlastCgerc(
    const CLBlastLayout            layout,
    const size_t            m,
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCgerc==NULL) {
        return 0;
    }
    return _g_CLBlastCgerc(
        layout,
        m,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZgerc)( /* CLBlastZgerc */
    const CLBlastLayout            /* layout */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZgerc _g_CLBlastZgerc = NULL;
CLBlastStatusCode CLBlastZgerc(
    const CLBlastLayout            layout,
    const size_t            m,
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZgerc==NULL) {
        return 0;
    }
    return _g_CLBlastZgerc(
        layout,
        m,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCher)( /* CLBlastCher */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCher _g_CLBlastCher = NULL;
CLBlastStatusCode CLBlastCher(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const float            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCher==NULL) {
        return 0;
    }
    return _g_CLBlastCher(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZher)( /* CLBlastZher */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZher _g_CLBlastZher = NULL;
CLBlastStatusCode CLBlastZher(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const double            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZher==NULL) {
        return 0;
    }
    return _g_CLBlastZher(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastChpr)( /* CLBlastChpr */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastChpr _g_CLBlastChpr = NULL;
CLBlastStatusCode CLBlastChpr(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const float            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastChpr==NULL) {
        return 0;
    }
    return _g_CLBlastChpr(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        ap_buffer,
        ap_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZhpr)( /* CLBlastZhpr */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZhpr _g_CLBlastZhpr = NULL;
CLBlastStatusCode CLBlastZhpr(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const double            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZhpr==NULL) {
        return 0;
    }
    return _g_CLBlastZhpr(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        ap_buffer,
        ap_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCher2)( /* CLBlastCher2 */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCher2 _g_CLBlastCher2 = NULL;
CLBlastStatusCode CLBlastCher2(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCher2==NULL) {
        return 0;
    }
    return _g_CLBlastCher2(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZher2)( /* CLBlastZher2 */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZher2 _g_CLBlastZher2 = NULL;
CLBlastStatusCode CLBlastZher2(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZher2==NULL) {
        return 0;
    }
    return _g_CLBlastZher2(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastChpr2)( /* CLBlastChpr2 */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastChpr2 _g_CLBlastChpr2 = NULL;
CLBlastStatusCode CLBlastChpr2(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastChpr2==NULL) {
        return 0;
    }
    return _g_CLBlastChpr2(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        ap_buffer,
        ap_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZhpr2)( /* CLBlastZhpr2 */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZhpr2 _g_CLBlastZhpr2 = NULL;
CLBlastStatusCode CLBlastZhpr2(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZhpr2==NULL) {
        return 0;
    }
    return _g_CLBlastZhpr2(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        ap_buffer,
        ap_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSsyr)( /* CLBlastSsyr */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSsyr _g_CLBlastSsyr = NULL;
CLBlastStatusCode CLBlastSsyr(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const float            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSsyr==NULL) {
        return 0;
    }
    return _g_CLBlastSsyr(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDsyr)( /* CLBlastDsyr */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDsyr _g_CLBlastDsyr = NULL;
CLBlastStatusCode CLBlastDsyr(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const double            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDsyr==NULL) {
        return 0;
    }
    return _g_CLBlastDsyr(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHsyr)( /* CLBlastHsyr */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHsyr _g_CLBlastHsyr = NULL;
CLBlastStatusCode CLBlastHsyr(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHsyr==NULL) {
        return 0;
    }
    return _g_CLBlastHsyr(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSspr)( /* CLBlastSspr */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSspr _g_CLBlastSspr = NULL;
CLBlastStatusCode CLBlastSspr(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const float            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSspr==NULL) {
        return 0;
    }
    return _g_CLBlastSspr(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        ap_buffer,
        ap_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDspr)( /* CLBlastDspr */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDspr _g_CLBlastDspr = NULL;
CLBlastStatusCode CLBlastDspr(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const double            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDspr==NULL) {
        return 0;
    }
    return _g_CLBlastDspr(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        ap_buffer,
        ap_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHspr)( /* CLBlastHspr */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHspr _g_CLBlastHspr = NULL;
CLBlastStatusCode CLBlastHspr(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHspr==NULL) {
        return 0;
    }
    return _g_CLBlastHspr(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        ap_buffer,
        ap_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSsyr2)( /* CLBlastSsyr2 */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSsyr2 _g_CLBlastSsyr2 = NULL;
CLBlastStatusCode CLBlastSsyr2(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const float            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSsyr2==NULL) {
        return 0;
    }
    return _g_CLBlastSsyr2(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDsyr2)( /* CLBlastDsyr2 */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDsyr2 _g_CLBlastDsyr2 = NULL;
CLBlastStatusCode CLBlastDsyr2(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const double            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDsyr2==NULL) {
        return 0;
    }
    return _g_CLBlastDsyr2(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHsyr2)( /* CLBlastHsyr2 */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHsyr2 _g_CLBlastHsyr2 = NULL;
CLBlastStatusCode CLBlastHsyr2(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHsyr2==NULL) {
        return 0;
    }
    return _g_CLBlastHsyr2(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        a_buffer,
        a_offset,
        a_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSspr2)( /* CLBlastSspr2 */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSspr2 _g_CLBlastSspr2 = NULL;
CLBlastStatusCode CLBlastSspr2(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const float            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSspr2==NULL) {
        return 0;
    }
    return _g_CLBlastSspr2(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        ap_buffer,
        ap_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDspr2)( /* CLBlastDspr2 */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDspr2 _g_CLBlastDspr2 = NULL;
CLBlastStatusCode CLBlastDspr2(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const double            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDspr2==NULL) {
        return 0;
    }
    return _g_CLBlastDspr2(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        ap_buffer,
        ap_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHspr2)( /* CLBlastHspr2 */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    cl_mem            /* ap_buffer */,
    const size_t            /* ap_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHspr2 _g_CLBlastHspr2 = NULL;
CLBlastStatusCode CLBlastHspr2(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    cl_mem            ap_buffer,
    const size_t            ap_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHspr2==NULL) {
        return 0;
    }
    return _g_CLBlastHspr2(
        layout,
        triangle,
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        ap_buffer,
        ap_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSgemm)( /* CLBlastSgemm */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const float            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSgemm _g_CLBlastSgemm = NULL;
CLBlastStatusCode CLBlastSgemm(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const float            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSgemm==NULL) {
        return 0;
    }
    return _g_CLBlastSgemm(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDgemm)( /* CLBlastDgemm */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const double            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDgemm _g_CLBlastDgemm = NULL;
CLBlastStatusCode CLBlastDgemm(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const double            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDgemm==NULL) {
        return 0;
    }
    return _g_CLBlastDgemm(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCgemm)( /* CLBlastCgemm */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_float2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCgemm _g_CLBlastCgemm = NULL;
CLBlastStatusCode CLBlastCgemm(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_float2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCgemm==NULL) {
        return 0;
    }
    return _g_CLBlastCgemm(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZgemm)( /* CLBlastZgemm */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_double2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZgemm _g_CLBlastZgemm = NULL;
CLBlastStatusCode CLBlastZgemm(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_double2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZgemm==NULL) {
        return 0;
    }
    return _g_CLBlastZgemm(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHgemm)( /* CLBlastHgemm */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_half            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_half            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHgemm _g_CLBlastHgemm = NULL;
CLBlastStatusCode CLBlastHgemm(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const cl_half            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_half            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHgemm==NULL) {
        return 0;
    }
    return _g_CLBlastHgemm(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSsymm)( /* CLBlastSsymm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* m */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const float            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSsymm _g_CLBlastSsymm = NULL;
CLBlastStatusCode CLBlastSsymm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const size_t            m,
    const size_t            n,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const float            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSsymm==NULL) {
        return 0;
    }
    return _g_CLBlastSsymm(
        layout,
        side,
        triangle,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDsymm)( /* CLBlastDsymm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* m */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const double            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDsymm _g_CLBlastDsymm = NULL;
CLBlastStatusCode CLBlastDsymm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const size_t            m,
    const size_t            n,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const double            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDsymm==NULL) {
        return 0;
    }
    return _g_CLBlastDsymm(
        layout,
        side,
        triangle,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCsymm)( /* CLBlastCsymm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_float2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCsymm _g_CLBlastCsymm = NULL;
CLBlastStatusCode CLBlastCsymm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const size_t            m,
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_float2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCsymm==NULL) {
        return 0;
    }
    return _g_CLBlastCsymm(
        layout,
        side,
        triangle,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZsymm)( /* CLBlastZsymm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_double2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZsymm _g_CLBlastZsymm = NULL;
CLBlastStatusCode CLBlastZsymm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const size_t            m,
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_double2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZsymm==NULL) {
        return 0;
    }
    return _g_CLBlastZsymm(
        layout,
        side,
        triangle,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHsymm)( /* CLBlastHsymm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_half            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHsymm _g_CLBlastHsymm = NULL;
CLBlastStatusCode CLBlastHsymm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const size_t            m,
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_half            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHsymm==NULL) {
        return 0;
    }
    return _g_CLBlastHsymm(
        layout,
        side,
        triangle,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastChemm)( /* CLBlastChemm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_float2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastChemm _g_CLBlastChemm = NULL;
CLBlastStatusCode CLBlastChemm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const size_t            m,
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_float2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastChemm==NULL) {
        return 0;
    }
    return _g_CLBlastChemm(
        layout,
        side,
        triangle,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZhemm)( /* CLBlastZhemm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_double2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZhemm _g_CLBlastZhemm = NULL;
CLBlastStatusCode CLBlastZhemm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const size_t            m,
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_double2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZhemm==NULL) {
        return 0;
    }
    return _g_CLBlastZhemm(
        layout,
        side,
        triangle,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSsyrk)( /* CLBlastSsyrk */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const float            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSsyrk _g_CLBlastSsyrk = NULL;
CLBlastStatusCode CLBlastSsyrk(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const size_t            n,
    const size_t            k,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const float            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSsyrk==NULL) {
        return 0;
    }
    return _g_CLBlastSsyrk(
        layout,
        triangle,
        a_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDsyrk)( /* CLBlastDsyrk */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const double            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDsyrk _g_CLBlastDsyrk = NULL;
CLBlastStatusCode CLBlastDsyrk(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const size_t            n,
    const size_t            k,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const double            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDsyrk==NULL) {
        return 0;
    }
    return _g_CLBlastDsyrk(
        layout,
        triangle,
        a_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCsyrk)( /* CLBlastCsyrk */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_float2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCsyrk _g_CLBlastCsyrk = NULL;
CLBlastStatusCode CLBlastCsyrk(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const size_t            n,
    const size_t            k,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_float2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCsyrk==NULL) {
        return 0;
    }
    return _g_CLBlastCsyrk(
        layout,
        triangle,
        a_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZsyrk)( /* CLBlastZsyrk */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_double2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZsyrk _g_CLBlastZsyrk = NULL;
CLBlastStatusCode CLBlastZsyrk(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const size_t            n,
    const size_t            k,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_double2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZsyrk==NULL) {
        return 0;
    }
    return _g_CLBlastZsyrk(
        layout,
        triangle,
        a_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHsyrk)( /* CLBlastHsyrk */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_half            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_half            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHsyrk _g_CLBlastHsyrk = NULL;
CLBlastStatusCode CLBlastHsyrk(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const size_t            n,
    const size_t            k,
    const cl_half            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_half            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHsyrk==NULL) {
        return 0;
    }
    return _g_CLBlastHsyrk(
        layout,
        triangle,
        a_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCherk)( /* CLBlastCherk */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const float            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCherk _g_CLBlastCherk = NULL;
CLBlastStatusCode CLBlastCherk(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const size_t            n,
    const size_t            k,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const float            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCherk==NULL) {
        return 0;
    }
    return _g_CLBlastCherk(
        layout,
        triangle,
        a_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZherk)( /* CLBlastZherk */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const double            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZherk _g_CLBlastZherk = NULL;
CLBlastStatusCode CLBlastZherk(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const size_t            n,
    const size_t            k,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const double            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZherk==NULL) {
        return 0;
    }
    return _g_CLBlastZherk(
        layout,
        triangle,
        a_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSsyr2k)( /* CLBlastSsyr2k */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* ab_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const float            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSsyr2k _g_CLBlastSsyr2k = NULL;
CLBlastStatusCode CLBlastSsyr2k(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            ab_transpose,
    const size_t            n,
    const size_t            k,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const float            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSsyr2k==NULL) {
        return 0;
    }
    return _g_CLBlastSsyr2k(
        layout,
        triangle,
        ab_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDsyr2k)( /* CLBlastDsyr2k */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* ab_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const double            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDsyr2k _g_CLBlastDsyr2k = NULL;
CLBlastStatusCode CLBlastDsyr2k(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            ab_transpose,
    const size_t            n,
    const size_t            k,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const double            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDsyr2k==NULL) {
        return 0;
    }
    return _g_CLBlastDsyr2k(
        layout,
        triangle,
        ab_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCsyr2k)( /* CLBlastCsyr2k */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* ab_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_float2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCsyr2k _g_CLBlastCsyr2k = NULL;
CLBlastStatusCode CLBlastCsyr2k(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            ab_transpose,
    const size_t            n,
    const size_t            k,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_float2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCsyr2k==NULL) {
        return 0;
    }
    return _g_CLBlastCsyr2k(
        layout,
        triangle,
        ab_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZsyr2k)( /* CLBlastZsyr2k */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* ab_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_double2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZsyr2k _g_CLBlastZsyr2k = NULL;
CLBlastStatusCode CLBlastZsyr2k(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            ab_transpose,
    const size_t            n,
    const size_t            k,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_double2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZsyr2k==NULL) {
        return 0;
    }
    return _g_CLBlastZsyr2k(
        layout,
        triangle,
        ab_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHsyr2k)( /* CLBlastHsyr2k */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* ab_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_half            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_half            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHsyr2k _g_CLBlastHsyr2k = NULL;
CLBlastStatusCode CLBlastHsyr2k(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            ab_transpose,
    const size_t            n,
    const size_t            k,
    const cl_half            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_half            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHsyr2k==NULL) {
        return 0;
    }
    return _g_CLBlastHsyr2k(
        layout,
        triangle,
        ab_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCher2k)( /* CLBlastCher2k */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* ab_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const float            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCher2k _g_CLBlastCher2k = NULL;
CLBlastStatusCode CLBlastCher2k(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            ab_transpose,
    const size_t            n,
    const size_t            k,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const float            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCher2k==NULL) {
        return 0;
    }
    return _g_CLBlastCher2k(
        layout,
        triangle,
        ab_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZher2k)( /* CLBlastZher2k */
    const CLBlastLayout            /* layout */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* ab_transpose */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const double            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZher2k _g_CLBlastZher2k = NULL;
CLBlastStatusCode CLBlastZher2k(
    const CLBlastLayout            layout,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            ab_transpose,
    const size_t            n,
    const size_t            k,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const double            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZher2k==NULL) {
        return 0;
    }
    return _g_CLBlastZher2k(
        layout,
        triangle,
        ab_transpose,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastStrmm)( /* CLBlastStrmm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* m */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastStrmm _g_CLBlastStrmm = NULL;
CLBlastStatusCode CLBlastStrmm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            m,
    const size_t            n,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastStrmm==NULL) {
        return 0;
    }
    return _g_CLBlastStrmm(
        layout,
        side,
        triangle,
        a_transpose,
        diagonal,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDtrmm)( /* CLBlastDtrmm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* m */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDtrmm _g_CLBlastDtrmm = NULL;
CLBlastStatusCode CLBlastDtrmm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            m,
    const size_t            n,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDtrmm==NULL) {
        return 0;
    }
    return _g_CLBlastDtrmm(
        layout,
        side,
        triangle,
        a_transpose,
        diagonal,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCtrmm)( /* CLBlastCtrmm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCtrmm _g_CLBlastCtrmm = NULL;
CLBlastStatusCode CLBlastCtrmm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            m,
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCtrmm==NULL) {
        return 0;
    }
    return _g_CLBlastCtrmm(
        layout,
        side,
        triangle,
        a_transpose,
        diagonal,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZtrmm)( /* CLBlastZtrmm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZtrmm _g_CLBlastZtrmm = NULL;
CLBlastStatusCode CLBlastZtrmm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            m,
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZtrmm==NULL) {
        return 0;
    }
    return _g_CLBlastZtrmm(
        layout,
        side,
        triangle,
        a_transpose,
        diagonal,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHtrmm)( /* CLBlastHtrmm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHtrmm _g_CLBlastHtrmm = NULL;
CLBlastStatusCode CLBlastHtrmm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            m,
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHtrmm==NULL) {
        return 0;
    }
    return _g_CLBlastHtrmm(
        layout,
        side,
        triangle,
        a_transpose,
        diagonal,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastStrsm)( /* CLBlastStrsm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* m */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastStrsm _g_CLBlastStrsm = NULL;
CLBlastStatusCode CLBlastStrsm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            m,
    const size_t            n,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastStrsm==NULL) {
        return 0;
    }
    return _g_CLBlastStrsm(
        layout,
        side,
        triangle,
        a_transpose,
        diagonal,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDtrsm)( /* CLBlastDtrsm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* m */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDtrsm _g_CLBlastDtrsm = NULL;
CLBlastStatusCode CLBlastDtrsm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            m,
    const size_t            n,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDtrsm==NULL) {
        return 0;
    }
    return _g_CLBlastDtrsm(
        layout,
        side,
        triangle,
        a_transpose,
        diagonal,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCtrsm)( /* CLBlastCtrsm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCtrsm _g_CLBlastCtrsm = NULL;
CLBlastStatusCode CLBlastCtrsm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            m,
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCtrsm==NULL) {
        return 0;
    }
    return _g_CLBlastCtrsm(
        layout,
        side,
        triangle,
        a_transpose,
        diagonal,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZtrsm)( /* CLBlastZtrsm */
    const CLBlastLayout            /* layout */,
    const CLBlastSide            /* side */,
    const CLBlastTriangle            /* triangle */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastDiagonal            /* diagonal */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZtrsm _g_CLBlastZtrsm = NULL;
CLBlastStatusCode CLBlastZtrsm(
    const CLBlastLayout            layout,
    const CLBlastSide            side,
    const CLBlastTriangle            triangle,
    const CLBlastTranspose            a_transpose,
    const CLBlastDiagonal            diagonal,
    const size_t            m,
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZtrsm==NULL) {
        return 0;
    }
    return _g_CLBlastZtrsm(
        layout,
        side,
        triangle,
        a_transpose,
        diagonal,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastShad)( /* CLBlastShad */
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    const float            /* beta */,
    cl_mem            /* z_buffer */,
    const size_t            /* z_offset */,
    const size_t            /* z_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastShad _g_CLBlastShad = NULL;
CLBlastStatusCode CLBlastShad(
    const size_t            n,
    const float            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    const float            beta,
    cl_mem            z_buffer,
    const size_t            z_offset,
    const size_t            z_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastShad==NULL) {
        return 0;
    }
    return _g_CLBlastShad(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        beta,
        z_buffer,
        z_offset,
        z_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDhad)( /* CLBlastDhad */
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    const double            /* beta */,
    cl_mem            /* z_buffer */,
    const size_t            /* z_offset */,
    const size_t            /* z_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDhad _g_CLBlastDhad = NULL;
CLBlastStatusCode CLBlastDhad(
    const size_t            n,
    const double            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    const double            beta,
    cl_mem            z_buffer,
    const size_t            z_offset,
    const size_t            z_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDhad==NULL) {
        return 0;
    }
    return _g_CLBlastDhad(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        beta,
        z_buffer,
        z_offset,
        z_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastChad)( /* CLBlastChad */
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    const cl_float2            /* beta */,
    cl_mem            /* z_buffer */,
    const size_t            /* z_offset */,
    const size_t            /* z_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastChad _g_CLBlastChad = NULL;
CLBlastStatusCode CLBlastChad(
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    const cl_float2            beta,
    cl_mem            z_buffer,
    const size_t            z_offset,
    const size_t            z_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastChad==NULL) {
        return 0;
    }
    return _g_CLBlastChad(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        beta,
        z_buffer,
        z_offset,
        z_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZhad)( /* CLBlastZhad */
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    const cl_double2            /* beta */,
    cl_mem            /* z_buffer */,
    const size_t            /* z_offset */,
    const size_t            /* z_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZhad _g_CLBlastZhad = NULL;
CLBlastStatusCode CLBlastZhad(
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    const cl_double2            beta,
    cl_mem            z_buffer,
    const size_t            z_offset,
    const size_t            z_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZhad==NULL) {
        return 0;
    }
    return _g_CLBlastZhad(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        beta,
        z_buffer,
        z_offset,
        z_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHhad)( /* CLBlastHhad */
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* x_buffer */,
    const size_t            /* x_offset */,
    const size_t            /* x_inc */,
    const cl_mem            /* y_buffer */,
    const size_t            /* y_offset */,
    const size_t            /* y_inc */,
    const cl_half            /* beta */,
    cl_mem            /* z_buffer */,
    const size_t            /* z_offset */,
    const size_t            /* z_inc */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHhad _g_CLBlastHhad = NULL;
CLBlastStatusCode CLBlastHhad(
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            x_buffer,
    const size_t            x_offset,
    const size_t            x_inc,
    const cl_mem            y_buffer,
    const size_t            y_offset,
    const size_t            y_inc,
    const cl_half            beta,
    cl_mem            z_buffer,
    const size_t            z_offset,
    const size_t            z_inc,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHhad==NULL) {
        return 0;
    }
    return _g_CLBlastHhad(
        n,
        alpha,
        x_buffer,
        x_offset,
        x_inc,
        y_buffer,
        y_offset,
        y_inc,
        beta,
        z_buffer,
        z_offset,
        z_inc,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSomatcopy)( /* CLBlastSomatcopy */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSomatcopy _g_CLBlastSomatcopy = NULL;
CLBlastStatusCode CLBlastSomatcopy(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSomatcopy==NULL) {
        return 0;
    }
    return _g_CLBlastSomatcopy(
        layout,
        a_transpose,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDomatcopy)( /* CLBlastDomatcopy */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDomatcopy _g_CLBlastDomatcopy = NULL;
CLBlastStatusCode CLBlastDomatcopy(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDomatcopy==NULL) {
        return 0;
    }
    return _g_CLBlastDomatcopy(
        layout,
        a_transpose,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastComatcopy)( /* CLBlastComatcopy */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastComatcopy _g_CLBlastComatcopy = NULL;
CLBlastStatusCode CLBlastComatcopy(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastComatcopy==NULL) {
        return 0;
    }
    return _g_CLBlastComatcopy(
        layout,
        a_transpose,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZomatcopy)( /* CLBlastZomatcopy */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZomatcopy _g_CLBlastZomatcopy = NULL;
CLBlastStatusCode CLBlastZomatcopy(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZomatcopy==NULL) {
        return 0;
    }
    return _g_CLBlastZomatcopy(
        layout,
        a_transpose,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHomatcopy)( /* CLBlastHomatcopy */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const cl_half            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHomatcopy _g_CLBlastHomatcopy = NULL;
CLBlastStatusCode CLBlastHomatcopy(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const size_t            m,
    const size_t            n,
    const cl_half            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHomatcopy==NULL) {
        return 0;
    }
    return _g_CLBlastHomatcopy(
        layout,
        a_transpose,
        m,
        n,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSim2col)( /* CLBlastSim2col */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    cl_mem            /* col_buffer */,
    const size_t            /* col_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSim2col _g_CLBlastSim2col = NULL;
CLBlastStatusCode CLBlastSim2col(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const cl_mem            im_buffer,
    const size_t            im_offset,
    cl_mem            col_buffer,
    const size_t            col_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSim2col==NULL) {
        return 0;
    }
    return _g_CLBlastSim2col(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        im_buffer,
        im_offset,
        col_buffer,
        col_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDim2col)( /* CLBlastDim2col */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    cl_mem            /* col_buffer */,
    const size_t            /* col_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDim2col _g_CLBlastDim2col = NULL;
CLBlastStatusCode CLBlastDim2col(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const cl_mem            im_buffer,
    const size_t            im_offset,
    cl_mem            col_buffer,
    const size_t            col_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDim2col==NULL) {
        return 0;
    }
    return _g_CLBlastDim2col(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        im_buffer,
        im_offset,
        col_buffer,
        col_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCim2col)( /* CLBlastCim2col */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    cl_mem            /* col_buffer */,
    const size_t            /* col_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCim2col _g_CLBlastCim2col = NULL;
CLBlastStatusCode CLBlastCim2col(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const cl_mem            im_buffer,
    const size_t            im_offset,
    cl_mem            col_buffer,
    const size_t            col_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCim2col==NULL) {
        return 0;
    }
    return _g_CLBlastCim2col(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        im_buffer,
        im_offset,
        col_buffer,
        col_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZim2col)( /* CLBlastZim2col */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    cl_mem            /* col_buffer */,
    const size_t            /* col_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZim2col _g_CLBlastZim2col = NULL;
CLBlastStatusCode CLBlastZim2col(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const cl_mem            im_buffer,
    const size_t            im_offset,
    cl_mem            col_buffer,
    const size_t            col_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZim2col==NULL) {
        return 0;
    }
    return _g_CLBlastZim2col(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        im_buffer,
        im_offset,
        col_buffer,
        col_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHim2col)( /* CLBlastHim2col */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    cl_mem            /* col_buffer */,
    const size_t            /* col_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHim2col _g_CLBlastHim2col = NULL;
CLBlastStatusCode CLBlastHim2col(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const cl_mem            im_buffer,
    const size_t            im_offset,
    cl_mem            col_buffer,
    const size_t            col_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHim2col==NULL) {
        return 0;
    }
    return _g_CLBlastHim2col(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        im_buffer,
        im_offset,
        col_buffer,
        col_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastScol2im)( /* CLBlastScol2im */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const cl_mem            /* col_buffer */,
    const size_t            /* col_offset */,
    cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastScol2im _g_CLBlastScol2im = NULL;
CLBlastStatusCode CLBlastScol2im(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const cl_mem            col_buffer,
    const size_t            col_offset,
    cl_mem            im_buffer,
    const size_t            im_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastScol2im==NULL) {
        return 0;
    }
    return _g_CLBlastScol2im(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        col_buffer,
        col_offset,
        im_buffer,
        im_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDcol2im)( /* CLBlastDcol2im */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const cl_mem            /* col_buffer */,
    const size_t            /* col_offset */,
    cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDcol2im _g_CLBlastDcol2im = NULL;
CLBlastStatusCode CLBlastDcol2im(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const cl_mem            col_buffer,
    const size_t            col_offset,
    cl_mem            im_buffer,
    const size_t            im_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDcol2im==NULL) {
        return 0;
    }
    return _g_CLBlastDcol2im(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        col_buffer,
        col_offset,
        im_buffer,
        im_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCcol2im)( /* CLBlastCcol2im */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const cl_mem            /* col_buffer */,
    const size_t            /* col_offset */,
    cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCcol2im _g_CLBlastCcol2im = NULL;
CLBlastStatusCode CLBlastCcol2im(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const cl_mem            col_buffer,
    const size_t            col_offset,
    cl_mem            im_buffer,
    const size_t            im_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCcol2im==NULL) {
        return 0;
    }
    return _g_CLBlastCcol2im(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        col_buffer,
        col_offset,
        im_buffer,
        im_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZcol2im)( /* CLBlastZcol2im */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const cl_mem            /* col_buffer */,
    const size_t            /* col_offset */,
    cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZcol2im _g_CLBlastZcol2im = NULL;
CLBlastStatusCode CLBlastZcol2im(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const cl_mem            col_buffer,
    const size_t            col_offset,
    cl_mem            im_buffer,
    const size_t            im_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZcol2im==NULL) {
        return 0;
    }
    return _g_CLBlastZcol2im(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        col_buffer,
        col_offset,
        im_buffer,
        im_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHcol2im)( /* CLBlastHcol2im */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const cl_mem            /* col_buffer */,
    const size_t            /* col_offset */,
    cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHcol2im _g_CLBlastHcol2im = NULL;
CLBlastStatusCode CLBlastHcol2im(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const cl_mem            col_buffer,
    const size_t            col_offset,
    cl_mem            im_buffer,
    const size_t            im_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHcol2im==NULL) {
        return 0;
    }
    return _g_CLBlastHcol2im(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        col_buffer,
        col_offset,
        im_buffer,
        im_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSconvgemm)( /* CLBlastSconvgemm */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const size_t            /* num_kernels */,
    const size_t            /* batch_count */,
    const cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    const cl_mem            /* kernel_buffer */,
    const size_t            /* kernel_offset */,
    cl_mem            /* result_buffer */,
    const size_t            /* result_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSconvgemm _g_CLBlastSconvgemm = NULL;
CLBlastStatusCode CLBlastSconvgemm(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const size_t            num_kernels,
    const size_t            batch_count,
    const cl_mem            im_buffer,
    const size_t            im_offset,
    const cl_mem            kernel_buffer,
    const size_t            kernel_offset,
    cl_mem            result_buffer,
    const size_t            result_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSconvgemm==NULL) {
        return 0;
    }
    return _g_CLBlastSconvgemm(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        num_kernels,
        batch_count,
        im_buffer,
        im_offset,
        kernel_buffer,
        kernel_offset,
        result_buffer,
        result_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDconvgemm)( /* CLBlastDconvgemm */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const size_t            /* num_kernels */,
    const size_t            /* batch_count */,
    const cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    const cl_mem            /* kernel_buffer */,
    const size_t            /* kernel_offset */,
    cl_mem            /* result_buffer */,
    const size_t            /* result_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDconvgemm _g_CLBlastDconvgemm = NULL;
CLBlastStatusCode CLBlastDconvgemm(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const size_t            num_kernels,
    const size_t            batch_count,
    const cl_mem            im_buffer,
    const size_t            im_offset,
    const cl_mem            kernel_buffer,
    const size_t            kernel_offset,
    cl_mem            result_buffer,
    const size_t            result_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDconvgemm==NULL) {
        return 0;
    }
    return _g_CLBlastDconvgemm(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        num_kernels,
        batch_count,
        im_buffer,
        im_offset,
        kernel_buffer,
        kernel_offset,
        result_buffer,
        result_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHconvgemm)( /* CLBlastHconvgemm */
    const CLBlastKernelMode            /* kernel_mode */,
    const size_t            /* channels */,
    const size_t            /* height */,
    const size_t            /* width */,
    const size_t            /* kernel_h */,
    const size_t            /* kernel_w */,
    const size_t            /* pad_h */,
    const size_t            /* pad_w */,
    const size_t            /* stride_h */,
    const size_t            /* stride_w */,
    const size_t            /* dilation_h */,
    const size_t            /* dilation_w */,
    const size_t            /* num_kernels */,
    const size_t            /* batch_count */,
    const cl_mem            /* im_buffer */,
    const size_t            /* im_offset */,
    const cl_mem            /* kernel_buffer */,
    const size_t            /* kernel_offset */,
    cl_mem            /* result_buffer */,
    const size_t            /* result_offset */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHconvgemm _g_CLBlastHconvgemm = NULL;
CLBlastStatusCode CLBlastHconvgemm(
    const CLBlastKernelMode            kernel_mode,
    const size_t            channels,
    const size_t            height,
    const size_t            width,
    const size_t            kernel_h,
    const size_t            kernel_w,
    const size_t            pad_h,
    const size_t            pad_w,
    const size_t            stride_h,
    const size_t            stride_w,
    const size_t            dilation_h,
    const size_t            dilation_w,
    const size_t            num_kernels,
    const size_t            batch_count,
    const cl_mem            im_buffer,
    const size_t            im_offset,
    const cl_mem            kernel_buffer,
    const size_t            kernel_offset,
    cl_mem            result_buffer,
    const size_t            result_offset,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHconvgemm==NULL) {
        return 0;
    }
    return _g_CLBlastHconvgemm(
        kernel_mode,
        channels,
        height,
        width,
        kernel_h,
        kernel_w,
        pad_h,
        pad_w,
        stride_h,
        stride_w,
        dilation_h,
        dilation_w,
        num_kernels,
        batch_count,
        im_buffer,
        im_offset,
        kernel_buffer,
        kernel_offset,
        result_buffer,
        result_offset,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSaxpyBatched)( /* CLBlastSaxpyBatched */
    const size_t            /* n */,
    const float *            /* alphas */,
    const cl_mem            /* x_buffer */,
    const size_t *            /* x_offsets */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t *            /* y_offsets */,
    const size_t            /* y_inc */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSaxpyBatched _g_CLBlastSaxpyBatched = NULL;
CLBlastStatusCode CLBlastSaxpyBatched(
    const size_t            n,
    const float *            alphas,
    const cl_mem            x_buffer,
    const size_t *            x_offsets,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t *            y_offsets,
    const size_t            y_inc,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSaxpyBatched==NULL) {
        return 0;
    }
    return _g_CLBlastSaxpyBatched(
        n,
        alphas,
        x_buffer,
        x_offsets,
        x_inc,
        y_buffer,
        y_offsets,
        y_inc,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDaxpyBatched)( /* CLBlastDaxpyBatched */
    const size_t            /* n */,
    const double *            /* alphas */,
    const cl_mem            /* x_buffer */,
    const size_t *            /* x_offsets */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t *            /* y_offsets */,
    const size_t            /* y_inc */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDaxpyBatched _g_CLBlastDaxpyBatched = NULL;
CLBlastStatusCode CLBlastDaxpyBatched(
    const size_t            n,
    const double *            alphas,
    const cl_mem            x_buffer,
    const size_t *            x_offsets,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t *            y_offsets,
    const size_t            y_inc,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDaxpyBatched==NULL) {
        return 0;
    }
    return _g_CLBlastDaxpyBatched(
        n,
        alphas,
        x_buffer,
        x_offsets,
        x_inc,
        y_buffer,
        y_offsets,
        y_inc,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCaxpyBatched)( /* CLBlastCaxpyBatched */
    const size_t            /* n */,
    const cl_float2 *            /* alphas */,
    const cl_mem            /* x_buffer */,
    const size_t *            /* x_offsets */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t *            /* y_offsets */,
    const size_t            /* y_inc */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCaxpyBatched _g_CLBlastCaxpyBatched = NULL;
CLBlastStatusCode CLBlastCaxpyBatched(
    const size_t            n,
    const cl_float2 *            alphas,
    const cl_mem            x_buffer,
    const size_t *            x_offsets,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t *            y_offsets,
    const size_t            y_inc,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCaxpyBatched==NULL) {
        return 0;
    }
    return _g_CLBlastCaxpyBatched(
        n,
        alphas,
        x_buffer,
        x_offsets,
        x_inc,
        y_buffer,
        y_offsets,
        y_inc,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZaxpyBatched)( /* CLBlastZaxpyBatched */
    const size_t            /* n */,
    const cl_double2 *            /* alphas */,
    const cl_mem            /* x_buffer */,
    const size_t *            /* x_offsets */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t *            /* y_offsets */,
    const size_t            /* y_inc */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZaxpyBatched _g_CLBlastZaxpyBatched = NULL;
CLBlastStatusCode CLBlastZaxpyBatched(
    const size_t            n,
    const cl_double2 *            alphas,
    const cl_mem            x_buffer,
    const size_t *            x_offsets,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t *            y_offsets,
    const size_t            y_inc,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZaxpyBatched==NULL) {
        return 0;
    }
    return _g_CLBlastZaxpyBatched(
        n,
        alphas,
        x_buffer,
        x_offsets,
        x_inc,
        y_buffer,
        y_offsets,
        y_inc,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHaxpyBatched)( /* CLBlastHaxpyBatched */
    const size_t            /* n */,
    const cl_half *            /* alphas */,
    const cl_mem            /* x_buffer */,
    const size_t *            /* x_offsets */,
    const size_t            /* x_inc */,
    cl_mem            /* y_buffer */,
    const size_t *            /* y_offsets */,
    const size_t            /* y_inc */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHaxpyBatched _g_CLBlastHaxpyBatched = NULL;
CLBlastStatusCode CLBlastHaxpyBatched(
    const size_t            n,
    const cl_half *            alphas,
    const cl_mem            x_buffer,
    const size_t *            x_offsets,
    const size_t            x_inc,
    cl_mem            y_buffer,
    const size_t *            y_offsets,
    const size_t            y_inc,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHaxpyBatched==NULL) {
        return 0;
    }
    return _g_CLBlastHaxpyBatched(
        n,
        alphas,
        x_buffer,
        x_offsets,
        x_inc,
        y_buffer,
        y_offsets,
        y_inc,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSgemmBatched)( /* CLBlastSgemmBatched */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const float *            /* alphas */,
    const cl_mem            /* a_buffer */,
    const size_t *            /* a_offsets */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t *            /* b_offsets */,
    const size_t            /* b_ld */,
    const float *            /* betas */,
    cl_mem            /* c_buffer */,
    const size_t *            /* c_offsets */,
    const size_t            /* c_ld */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSgemmBatched _g_CLBlastSgemmBatched = NULL;
CLBlastStatusCode CLBlastSgemmBatched(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const float *            alphas,
    const cl_mem            a_buffer,
    const size_t *            a_offsets,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t *            b_offsets,
    const size_t            b_ld,
    const float *            betas,
    cl_mem            c_buffer,
    const size_t *            c_offsets,
    const size_t            c_ld,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSgemmBatched==NULL) {
        return 0;
    }
    return _g_CLBlastSgemmBatched(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alphas,
        a_buffer,
        a_offsets,
        a_ld,
        b_buffer,
        b_offsets,
        b_ld,
        betas,
        c_buffer,
        c_offsets,
        c_ld,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDgemmBatched)( /* CLBlastDgemmBatched */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const double *            /* alphas */,
    const cl_mem            /* a_buffer */,
    const size_t *            /* a_offsets */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t *            /* b_offsets */,
    const size_t            /* b_ld */,
    const double *            /* betas */,
    cl_mem            /* c_buffer */,
    const size_t *            /* c_offsets */,
    const size_t            /* c_ld */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDgemmBatched _g_CLBlastDgemmBatched = NULL;
CLBlastStatusCode CLBlastDgemmBatched(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const double *            alphas,
    const cl_mem            a_buffer,
    const size_t *            a_offsets,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t *            b_offsets,
    const size_t            b_ld,
    const double *            betas,
    cl_mem            c_buffer,
    const size_t *            c_offsets,
    const size_t            c_ld,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDgemmBatched==NULL) {
        return 0;
    }
    return _g_CLBlastDgemmBatched(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alphas,
        a_buffer,
        a_offsets,
        a_ld,
        b_buffer,
        b_offsets,
        b_ld,
        betas,
        c_buffer,
        c_offsets,
        c_ld,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCgemmBatched)( /* CLBlastCgemmBatched */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_float2 *            /* alphas */,
    const cl_mem            /* a_buffer */,
    const size_t *            /* a_offsets */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t *            /* b_offsets */,
    const size_t            /* b_ld */,
    const cl_float2 *            /* betas */,
    cl_mem            /* c_buffer */,
    const size_t *            /* c_offsets */,
    const size_t            /* c_ld */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCgemmBatched _g_CLBlastCgemmBatched = NULL;
CLBlastStatusCode CLBlastCgemmBatched(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const cl_float2 *            alphas,
    const cl_mem            a_buffer,
    const size_t *            a_offsets,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t *            b_offsets,
    const size_t            b_ld,
    const cl_float2 *            betas,
    cl_mem            c_buffer,
    const size_t *            c_offsets,
    const size_t            c_ld,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCgemmBatched==NULL) {
        return 0;
    }
    return _g_CLBlastCgemmBatched(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alphas,
        a_buffer,
        a_offsets,
        a_ld,
        b_buffer,
        b_offsets,
        b_ld,
        betas,
        c_buffer,
        c_offsets,
        c_ld,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZgemmBatched)( /* CLBlastZgemmBatched */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_double2 *            /* alphas */,
    const cl_mem            /* a_buffer */,
    const size_t *            /* a_offsets */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t *            /* b_offsets */,
    const size_t            /* b_ld */,
    const cl_double2 *            /* betas */,
    cl_mem            /* c_buffer */,
    const size_t *            /* c_offsets */,
    const size_t            /* c_ld */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZgemmBatched _g_CLBlastZgemmBatched = NULL;
CLBlastStatusCode CLBlastZgemmBatched(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const cl_double2 *            alphas,
    const cl_mem            a_buffer,
    const size_t *            a_offsets,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t *            b_offsets,
    const size_t            b_ld,
    const cl_double2 *            betas,
    cl_mem            c_buffer,
    const size_t *            c_offsets,
    const size_t            c_ld,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZgemmBatched==NULL) {
        return 0;
    }
    return _g_CLBlastZgemmBatched(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alphas,
        a_buffer,
        a_offsets,
        a_ld,
        b_buffer,
        b_offsets,
        b_ld,
        betas,
        c_buffer,
        c_offsets,
        c_ld,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHgemmBatched)( /* CLBlastHgemmBatched */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_half *            /* alphas */,
    const cl_mem            /* a_buffer */,
    const size_t *            /* a_offsets */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t *            /* b_offsets */,
    const size_t            /* b_ld */,
    const cl_half *            /* betas */,
    cl_mem            /* c_buffer */,
    const size_t *            /* c_offsets */,
    const size_t            /* c_ld */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHgemmBatched _g_CLBlastHgemmBatched = NULL;
CLBlastStatusCode CLBlastHgemmBatched(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const cl_half *            alphas,
    const cl_mem            a_buffer,
    const size_t *            a_offsets,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t *            b_offsets,
    const size_t            b_ld,
    const cl_half *            betas,
    cl_mem            c_buffer,
    const size_t *            c_offsets,
    const size_t            c_ld,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHgemmBatched==NULL) {
        return 0;
    }
    return _g_CLBlastHgemmBatched(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alphas,
        a_buffer,
        a_offsets,
        a_ld,
        b_buffer,
        b_offsets,
        b_ld,
        betas,
        c_buffer,
        c_offsets,
        c_ld,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSgemmStridedBatched)( /* CLBlastSgemmStridedBatched */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const size_t            /* a_stride */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const size_t            /* b_stride */,
    const float            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    const size_t            /* c_stride */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastSgemmStridedBatched _g_CLBlastSgemmStridedBatched = NULL;
CLBlastStatusCode CLBlastSgemmStridedBatched(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const size_t            a_stride,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const size_t            b_stride,
    const float            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    const size_t            c_stride,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastSgemmStridedBatched==NULL) {
        return 0;
    }
    return _g_CLBlastSgemmStridedBatched(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        a_stride,
        b_buffer,
        b_offset,
        b_ld,
        b_stride,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        c_stride,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDgemmStridedBatched)( /* CLBlastDgemmStridedBatched */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const size_t            /* a_stride */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const size_t            /* b_stride */,
    const double            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    const size_t            /* c_stride */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastDgemmStridedBatched _g_CLBlastDgemmStridedBatched = NULL;
CLBlastStatusCode CLBlastDgemmStridedBatched(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const size_t            a_stride,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const size_t            b_stride,
    const double            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    const size_t            c_stride,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastDgemmStridedBatched==NULL) {
        return 0;
    }
    return _g_CLBlastDgemmStridedBatched(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        a_stride,
        b_buffer,
        b_offset,
        b_ld,
        b_stride,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        c_stride,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCgemmStridedBatched)( /* CLBlastCgemmStridedBatched */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const size_t            /* a_stride */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const size_t            /* b_stride */,
    const cl_float2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    const size_t            /* c_stride */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastCgemmStridedBatched _g_CLBlastCgemmStridedBatched = NULL;
CLBlastStatusCode CLBlastCgemmStridedBatched(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const size_t            a_stride,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const size_t            b_stride,
    const cl_float2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    const size_t            c_stride,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastCgemmStridedBatched==NULL) {
        return 0;
    }
    return _g_CLBlastCgemmStridedBatched(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        a_stride,
        b_buffer,
        b_offset,
        b_ld,
        b_stride,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        c_stride,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZgemmStridedBatched)( /* CLBlastZgemmStridedBatched */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const size_t            /* a_stride */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const size_t            /* b_stride */,
    const cl_double2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    const size_t            /* c_stride */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastZgemmStridedBatched _g_CLBlastZgemmStridedBatched = NULL;
CLBlastStatusCode CLBlastZgemmStridedBatched(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const size_t            a_stride,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const size_t            b_stride,
    const cl_double2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    const size_t            c_stride,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastZgemmStridedBatched==NULL) {
        return 0;
    }
    return _g_CLBlastZgemmStridedBatched(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        a_stride,
        b_buffer,
        b_offset,
        b_ld,
        b_stride,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        c_stride,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHgemmStridedBatched)( /* CLBlastHgemmStridedBatched */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_half            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const size_t            /* a_stride */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const size_t            /* b_stride */,
    const cl_half            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    const size_t            /* c_stride */,
    const size_t            /* batch_count */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */
);
static PFNCLBlastHgemmStridedBatched _g_CLBlastHgemmStridedBatched = NULL;
CLBlastStatusCode CLBlastHgemmStridedBatched(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const cl_half            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const size_t            a_stride,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const size_t            b_stride,
    const cl_half            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    const size_t            c_stride,
    const size_t            batch_count,
    cl_command_queue *            queue,
    cl_event *            event
)
{
    if(_h_clblast==NULL || _g_CLBlastHgemmStridedBatched==NULL) {
        return 0;
    }
    return _g_CLBlastHgemmStridedBatched(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        a_stride,
        b_buffer,
        b_offset,
        b_ld,
        b_stride,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        c_stride,
        batch_count,
        queue,
        event    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSgemmWithTempBuffer)( /* CLBlastSgemmWithTempBuffer */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const float            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const float            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */,
    cl_mem            /* temp_buffer */
);
static PFNCLBlastSgemmWithTempBuffer _g_CLBlastSgemmWithTempBuffer = NULL;
CLBlastStatusCode CLBlastSgemmWithTempBuffer(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const float            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const float            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event,
    cl_mem            temp_buffer
)
{
    if(_h_clblast==NULL || _g_CLBlastSgemmWithTempBuffer==NULL) {
        return 0;
    }
    return _g_CLBlastSgemmWithTempBuffer(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event,
        temp_buffer    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDgemmWithTempBuffer)( /* CLBlastDgemmWithTempBuffer */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const double            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const double            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */,
    cl_mem            /* temp_buffer */
);
static PFNCLBlastDgemmWithTempBuffer _g_CLBlastDgemmWithTempBuffer = NULL;
CLBlastStatusCode CLBlastDgemmWithTempBuffer(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const double            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const double            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event,
    cl_mem            temp_buffer
)
{
    if(_h_clblast==NULL || _g_CLBlastDgemmWithTempBuffer==NULL) {
        return 0;
    }
    return _g_CLBlastDgemmWithTempBuffer(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event,
        temp_buffer    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCgemmWithTempBuffer)( /* CLBlastCgemmWithTempBuffer */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_float2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_float2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */,
    cl_mem            /* temp_buffer */
);
static PFNCLBlastCgemmWithTempBuffer _g_CLBlastCgemmWithTempBuffer = NULL;
CLBlastStatusCode CLBlastCgemmWithTempBuffer(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const cl_float2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_float2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event,
    cl_mem            temp_buffer
)
{
    if(_h_clblast==NULL || _g_CLBlastCgemmWithTempBuffer==NULL) {
        return 0;
    }
    return _g_CLBlastCgemmWithTempBuffer(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event,
        temp_buffer    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZgemmWithTempBuffer)( /* CLBlastZgemmWithTempBuffer */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_double2            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_double2            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */,
    cl_mem            /* temp_buffer */
);
static PFNCLBlastZgemmWithTempBuffer _g_CLBlastZgemmWithTempBuffer = NULL;
CLBlastStatusCode CLBlastZgemmWithTempBuffer(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const cl_double2            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_double2            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event,
    cl_mem            temp_buffer
)
{
    if(_h_clblast==NULL || _g_CLBlastZgemmWithTempBuffer==NULL) {
        return 0;
    }
    return _g_CLBlastZgemmWithTempBuffer(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event,
        temp_buffer    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHgemmWithTempBuffer)( /* CLBlastHgemmWithTempBuffer */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const cl_half            /* alpha */,
    const cl_mem            /* a_buffer */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const cl_mem            /* b_buffer */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const cl_half            /* beta */,
    cl_mem            /* c_buffer */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    cl_event *            /* event */,
    cl_mem            /* temp_buffer */
);
static PFNCLBlastHgemmWithTempBuffer _g_CLBlastHgemmWithTempBuffer = NULL;
CLBlastStatusCode CLBlastHgemmWithTempBuffer(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const cl_half            alpha,
    const cl_mem            a_buffer,
    const size_t            a_offset,
    const size_t            a_ld,
    const cl_mem            b_buffer,
    const size_t            b_offset,
    const size_t            b_ld,
    const cl_half            beta,
    cl_mem            c_buffer,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    cl_event *            event,
    cl_mem            temp_buffer
)
{
    if(_h_clblast==NULL || _g_CLBlastHgemmWithTempBuffer==NULL) {
        return 0;
    }
    return _g_CLBlastHgemmWithTempBuffer(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        alpha,
        a_buffer,
        a_offset,
        a_ld,
        b_buffer,
        b_offset,
        b_ld,
        beta,
        c_buffer,
        c_offset,
        c_ld,
        queue,
        event,
        temp_buffer    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastSGemmTempBufferSize)( /* CLBlastSGemmTempBufferSize */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    size_t *            /* temp_buffer_size */
);
static PFNCLBlastSGemmTempBufferSize _g_CLBlastSGemmTempBufferSize = NULL;
CLBlastStatusCode CLBlastSGemmTempBufferSize(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const size_t            a_offset,
    const size_t            a_ld,
    const size_t            b_offset,
    const size_t            b_ld,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    size_t *            temp_buffer_size
)
{
    if(_h_clblast==NULL || _g_CLBlastSGemmTempBufferSize==NULL) {
        return 0;
    }
    return _g_CLBlastSGemmTempBufferSize(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        a_offset,
        a_ld,
        b_offset,
        b_ld,
        c_offset,
        c_ld,
        queue,
        temp_buffer_size    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastDGemmTempBufferSize)( /* CLBlastDGemmTempBufferSize */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    size_t *            /* temp_buffer_size */
);
static PFNCLBlastDGemmTempBufferSize _g_CLBlastDGemmTempBufferSize = NULL;
CLBlastStatusCode CLBlastDGemmTempBufferSize(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const size_t            a_offset,
    const size_t            a_ld,
    const size_t            b_offset,
    const size_t            b_ld,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    size_t *            temp_buffer_size
)
{
    if(_h_clblast==NULL || _g_CLBlastDGemmTempBufferSize==NULL) {
        return 0;
    }
    return _g_CLBlastDGemmTempBufferSize(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        a_offset,
        a_ld,
        b_offset,
        b_ld,
        c_offset,
        c_ld,
        queue,
        temp_buffer_size    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastCGemmTempBufferSize)( /* CLBlastCGemmTempBufferSize */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    size_t *            /* temp_buffer_size */
);
static PFNCLBlastCGemmTempBufferSize _g_CLBlastCGemmTempBufferSize = NULL;
CLBlastStatusCode CLBlastCGemmTempBufferSize(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const size_t            a_offset,
    const size_t            a_ld,
    const size_t            b_offset,
    const size_t            b_ld,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    size_t *            temp_buffer_size
)
{
    if(_h_clblast==NULL || _g_CLBlastCGemmTempBufferSize==NULL) {
        return 0;
    }
    return _g_CLBlastCGemmTempBufferSize(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        a_offset,
        a_ld,
        b_offset,
        b_ld,
        c_offset,
        c_ld,
        queue,
        temp_buffer_size    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastZGemmTempBufferSize)( /* CLBlastZGemmTempBufferSize */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    size_t *            /* temp_buffer_size */
);
static PFNCLBlastZGemmTempBufferSize _g_CLBlastZGemmTempBufferSize = NULL;
CLBlastStatusCode CLBlastZGemmTempBufferSize(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const size_t            a_offset,
    const size_t            a_ld,
    const size_t            b_offset,
    const size_t            b_ld,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    size_t *            temp_buffer_size
)
{
    if(_h_clblast==NULL || _g_CLBlastZGemmTempBufferSize==NULL) {
        return 0;
    }
    return _g_CLBlastZGemmTempBufferSize(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        a_offset,
        a_ld,
        b_offset,
        b_ld,
        c_offset,
        c_ld,
        queue,
        temp_buffer_size    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastHGemmTempBufferSize)( /* CLBlastHGemmTempBufferSize */
    const CLBlastLayout            /* layout */,
    const CLBlastTranspose            /* a_transpose */,
    const CLBlastTranspose            /* b_transpose */,
    const size_t            /* m */,
    const size_t            /* n */,
    const size_t            /* k */,
    const size_t            /* a_offset */,
    const size_t            /* a_ld */,
    const size_t            /* b_offset */,
    const size_t            /* b_ld */,
    const size_t            /* c_offset */,
    const size_t            /* c_ld */,
    cl_command_queue *            /* queue */,
    size_t *            /* temp_buffer_size */
);
static PFNCLBlastHGemmTempBufferSize _g_CLBlastHGemmTempBufferSize = NULL;
CLBlastStatusCode CLBlastHGemmTempBufferSize(
    const CLBlastLayout            layout,
    const CLBlastTranspose            a_transpose,
    const CLBlastTranspose            b_transpose,
    const size_t            m,
    const size_t            n,
    const size_t            k,
    const size_t            a_offset,
    const size_t            a_ld,
    const size_t            b_offset,
    const size_t            b_ld,
    const size_t            c_offset,
    const size_t            c_ld,
    cl_command_queue *            queue,
    size_t *            temp_buffer_size
)
{
    if(_h_clblast==NULL || _g_CLBlastHGemmTempBufferSize==NULL) {
        return 0;
    }
    return _g_CLBlastHGemmTempBufferSize(
        layout,
        a_transpose,
        b_transpose,
        m,
        n,
        k,
        a_offset,
        a_ld,
        b_offset,
        b_ld,
        c_offset,
        c_ld,
        queue,
        temp_buffer_size    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastClearCache)( /* CLBlastClearCache */
                /*  */
);
static PFNCLBlastClearCache _g_CLBlastClearCache = NULL;
CLBlastStatusCode CLBlastClearCache(
                
)
{
    if(_h_clblast==NULL || _g_CLBlastClearCache==NULL) {
        return 0;
    }
    return _g_CLBlastClearCache(
    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastFillCache)( /* CLBlastFillCache */
    const cl_device_id            /* device */
);
static PFNCLBlastFillCache _g_CLBlastFillCache = NULL;
CLBlastStatusCode CLBlastFillCache(
    const cl_device_id            device
)
{
    if(_h_clblast==NULL || _g_CLBlastFillCache==NULL) {
        return 0;
    }
    return _g_CLBlastFillCache(
        device    
    );
}
typedef CLBlastStatusCode (CALLBACK* PFNCLBlastOverrideParameters)( /* CLBlastOverrideParameters */
    const cl_device_id            /* device */,
    const char *            /* kernel_name */,
    const CLBlastPrecision            /* precision */,
    const size_t            /* num_parameters */,
    const char * *            /* parameters_names */,
    const size_t *            /* parameters_values */
);
static PFNCLBlastOverrideParameters _g_CLBlastOverrideParameters = NULL;
CLBlastStatusCode CLBlastOverrideParameters(
    const cl_device_id            device,
    const char *            kernel_name,
    const CLBlastPrecision            precision,
    const size_t            num_parameters,
    const char * *            parameters_names,
    const size_t *            parameters_values
)
{
    if(_h_clblast==NULL || _g_CLBlastOverrideParameters==NULL) {
        return 0;
    }
    return _g_CLBlastOverrideParameters(
        device,
        kernel_name,
        precision,
        num_parameters,
        parameters_names,
        parameters_values    
    );
}
int rindow_load_clblast_dll()
{
    if(_h_clblast!=NULL) {
        return 0;
    }
    _h_clblast = LoadLibraryA( "clblast.dll" );
    if(_h_clblast==NULL) {
        printf("load error: clblast\n");
        return -1;
    }
    LOADFUNC(CLBlastSrotg)
    LOADFUNC(CLBlastDrotg)
    LOADFUNC(CLBlastSrotmg)
    LOADFUNC(CLBlastDrotmg)
    LOADFUNC(CLBlastSrot)
    LOADFUNC(CLBlastDrot)
    LOADFUNC(CLBlastSrotm)
    LOADFUNC(CLBlastDrotm)
    LOADFUNC(CLBlastSswap)
    LOADFUNC(CLBlastDswap)
    LOADFUNC(CLBlastCswap)
    LOADFUNC(CLBlastZswap)
    LOADFUNC(CLBlastHswap)
    LOADFUNC(CLBlastSscal)
    LOADFUNC(CLBlastDscal)
    LOADFUNC(CLBlastCscal)
    LOADFUNC(CLBlastZscal)
    LOADFUNC(CLBlastHscal)
    LOADFUNC(CLBlastScopy)
    LOADFUNC(CLBlastDcopy)
    LOADFUNC(CLBlastCcopy)
    LOADFUNC(CLBlastZcopy)
    LOADFUNC(CLBlastHcopy)
    LOADFUNC(CLBlastSaxpy)
    LOADFUNC(CLBlastDaxpy)
    LOADFUNC(CLBlastCaxpy)
    LOADFUNC(CLBlastZaxpy)
    LOADFUNC(CLBlastHaxpy)
    LOADFUNC(CLBlastSdot)
    LOADFUNC(CLBlastDdot)
    LOADFUNC(CLBlastHdot)
    LOADFUNC(CLBlastCdotu)
    LOADFUNC(CLBlastZdotu)
    LOADFUNC(CLBlastCdotc)
    LOADFUNC(CLBlastZdotc)
    LOADFUNC(CLBlastSnrm2)
    LOADFUNC(CLBlastDnrm2)
    LOADFUNC(CLBlastScnrm2)
    LOADFUNC(CLBlastDznrm2)
    LOADFUNC(CLBlastHnrm2)
    LOADFUNC(CLBlastSasum)
    LOADFUNC(CLBlastDasum)
    LOADFUNC(CLBlastScasum)
    LOADFUNC(CLBlastDzasum)
    LOADFUNC(CLBlastHasum)
    LOADFUNC(CLBlastSsum)
    LOADFUNC(CLBlastDsum)
    LOADFUNC(CLBlastScsum)
    LOADFUNC(CLBlastDzsum)
    LOADFUNC(CLBlastHsum)
    LOADFUNC(CLBlastiSamax)
    LOADFUNC(CLBlastiDamax)
    LOADFUNC(CLBlastiCamax)
    LOADFUNC(CLBlastiZamax)
    LOADFUNC(CLBlastiHamax)
    LOADFUNC(CLBlastiSamin)
    LOADFUNC(CLBlastiDamin)
    LOADFUNC(CLBlastiCamin)
    LOADFUNC(CLBlastiZamin)
    LOADFUNC(CLBlastiHamin)
    LOADFUNC(CLBlastiSmax)
    LOADFUNC(CLBlastiDmax)
    LOADFUNC(CLBlastiCmax)
    LOADFUNC(CLBlastiZmax)
    LOADFUNC(CLBlastiHmax)
    LOADFUNC(CLBlastiSmin)
    LOADFUNC(CLBlastiDmin)
    LOADFUNC(CLBlastiCmin)
    LOADFUNC(CLBlastiZmin)
    LOADFUNC(CLBlastiHmin)
    LOADFUNC(CLBlastSgemv)
    LOADFUNC(CLBlastDgemv)
    LOADFUNC(CLBlastCgemv)
    LOADFUNC(CLBlastZgemv)
    LOADFUNC(CLBlastHgemv)
    LOADFUNC(CLBlastSgbmv)
    LOADFUNC(CLBlastDgbmv)
    LOADFUNC(CLBlastCgbmv)
    LOADFUNC(CLBlastZgbmv)
    LOADFUNC(CLBlastHgbmv)
    LOADFUNC(CLBlastChemv)
    LOADFUNC(CLBlastZhemv)
    LOADFUNC(CLBlastChbmv)
    LOADFUNC(CLBlastZhbmv)
    LOADFUNC(CLBlastChpmv)
    LOADFUNC(CLBlastZhpmv)
    LOADFUNC(CLBlastSsymv)
    LOADFUNC(CLBlastDsymv)
    LOADFUNC(CLBlastHsymv)
    LOADFUNC(CLBlastSsbmv)
    LOADFUNC(CLBlastDsbmv)
    LOADFUNC(CLBlastHsbmv)
    LOADFUNC(CLBlastSspmv)
    LOADFUNC(CLBlastDspmv)
    LOADFUNC(CLBlastHspmv)
    LOADFUNC(CLBlastStrmv)
    LOADFUNC(CLBlastDtrmv)
    LOADFUNC(CLBlastCtrmv)
    LOADFUNC(CLBlastZtrmv)
    LOADFUNC(CLBlastHtrmv)
    LOADFUNC(CLBlastStbmv)
    LOADFUNC(CLBlastDtbmv)
    LOADFUNC(CLBlastCtbmv)
    LOADFUNC(CLBlastZtbmv)
    LOADFUNC(CLBlastHtbmv)
    LOADFUNC(CLBlastStpmv)
    LOADFUNC(CLBlastDtpmv)
    LOADFUNC(CLBlastCtpmv)
    LOADFUNC(CLBlastZtpmv)
    LOADFUNC(CLBlastHtpmv)
    LOADFUNC(CLBlastStrsv)
    LOADFUNC(CLBlastDtrsv)
    LOADFUNC(CLBlastCtrsv)
    LOADFUNC(CLBlastZtrsv)
    LOADFUNC(CLBlastStbsv)
    LOADFUNC(CLBlastDtbsv)
    LOADFUNC(CLBlastCtbsv)
    LOADFUNC(CLBlastZtbsv)
    LOADFUNC(CLBlastStpsv)
    LOADFUNC(CLBlastDtpsv)
    LOADFUNC(CLBlastCtpsv)
    LOADFUNC(CLBlastZtpsv)
    LOADFUNC(CLBlastSger)
    LOADFUNC(CLBlastDger)
    LOADFUNC(CLBlastHger)
    LOADFUNC(CLBlastCgeru)
    LOADFUNC(CLBlastZgeru)
    LOADFUNC(CLBlastCgerc)
    LOADFUNC(CLBlastZgerc)
    LOADFUNC(CLBlastCher)
    LOADFUNC(CLBlastZher)
    LOADFUNC(CLBlastChpr)
    LOADFUNC(CLBlastZhpr)
    LOADFUNC(CLBlastCher2)
    LOADFUNC(CLBlastZher2)
    LOADFUNC(CLBlastChpr2)
    LOADFUNC(CLBlastZhpr2)
    LOADFUNC(CLBlastSsyr)
    LOADFUNC(CLBlastDsyr)
    LOADFUNC(CLBlastHsyr)
    LOADFUNC(CLBlastSspr)
    LOADFUNC(CLBlastDspr)
    LOADFUNC(CLBlastHspr)
    LOADFUNC(CLBlastSsyr2)
    LOADFUNC(CLBlastDsyr2)
    LOADFUNC(CLBlastHsyr2)
    LOADFUNC(CLBlastSspr2)
    LOADFUNC(CLBlastDspr2)
    LOADFUNC(CLBlastHspr2)
    LOADFUNC(CLBlastSgemm)
    LOADFUNC(CLBlastDgemm)
    LOADFUNC(CLBlastCgemm)
    LOADFUNC(CLBlastZgemm)
    LOADFUNC(CLBlastHgemm)
    LOADFUNC(CLBlastSsymm)
    LOADFUNC(CLBlastDsymm)
    LOADFUNC(CLBlastCsymm)
    LOADFUNC(CLBlastZsymm)
    LOADFUNC(CLBlastHsymm)
    LOADFUNC(CLBlastChemm)
    LOADFUNC(CLBlastZhemm)
    LOADFUNC(CLBlastSsyrk)
    LOADFUNC(CLBlastDsyrk)
    LOADFUNC(CLBlastCsyrk)
    LOADFUNC(CLBlastZsyrk)
    LOADFUNC(CLBlastHsyrk)
    LOADFUNC(CLBlastCherk)
    LOADFUNC(CLBlastZherk)
    LOADFUNC(CLBlastSsyr2k)
    LOADFUNC(CLBlastDsyr2k)
    LOADFUNC(CLBlastCsyr2k)
    LOADFUNC(CLBlastZsyr2k)
    LOADFUNC(CLBlastHsyr2k)
    LOADFUNC(CLBlastCher2k)
    LOADFUNC(CLBlastZher2k)
    LOADFUNC(CLBlastStrmm)
    LOADFUNC(CLBlastDtrmm)
    LOADFUNC(CLBlastCtrmm)
    LOADFUNC(CLBlastZtrmm)
    LOADFUNC(CLBlastHtrmm)
    LOADFUNC(CLBlastStrsm)
    LOADFUNC(CLBlastDtrsm)
    LOADFUNC(CLBlastCtrsm)
    LOADFUNC(CLBlastZtrsm)
    LOADFUNC(CLBlastShad)
    LOADFUNC(CLBlastDhad)
    LOADFUNC(CLBlastChad)
    LOADFUNC(CLBlastZhad)
    LOADFUNC(CLBlastHhad)
    LOADFUNC(CLBlastSomatcopy)
    LOADFUNC(CLBlastDomatcopy)
    LOADFUNC(CLBlastComatcopy)
    LOADFUNC(CLBlastZomatcopy)
    LOADFUNC(CLBlastHomatcopy)
    LOADFUNC(CLBlastSim2col)
    LOADFUNC(CLBlastDim2col)
    LOADFUNC(CLBlastCim2col)
    LOADFUNC(CLBlastZim2col)
    LOADFUNC(CLBlastHim2col)
    LOADFUNC(CLBlastScol2im)
    LOADFUNC(CLBlastDcol2im)
    LOADFUNC(CLBlastCcol2im)
    LOADFUNC(CLBlastZcol2im)
    LOADFUNC(CLBlastHcol2im)
    LOADFUNC(CLBlastSconvgemm)
    LOADFUNC(CLBlastDconvgemm)
    LOADFUNC(CLBlastHconvgemm)
    LOADFUNC(CLBlastSaxpyBatched)
    LOADFUNC(CLBlastDaxpyBatched)
    LOADFUNC(CLBlastCaxpyBatched)
    LOADFUNC(CLBlastZaxpyBatched)
    LOADFUNC(CLBlastHaxpyBatched)
    LOADFUNC(CLBlastSgemmBatched)
    LOADFUNC(CLBlastDgemmBatched)
    LOADFUNC(CLBlastCgemmBatched)
    LOADFUNC(CLBlastZgemmBatched)
    LOADFUNC(CLBlastHgemmBatched)
    LOADFUNC(CLBlastSgemmStridedBatched)
    LOADFUNC(CLBlastDgemmStridedBatched)
    LOADFUNC(CLBlastCgemmStridedBatched)
    LOADFUNC(CLBlastZgemmStridedBatched)
    LOADFUNC(CLBlastHgemmStridedBatched)
    LOADFUNC(CLBlastSgemmWithTempBuffer)
    LOADFUNC(CLBlastDgemmWithTempBuffer)
    LOADFUNC(CLBlastCgemmWithTempBuffer)
    LOADFUNC(CLBlastZgemmWithTempBuffer)
    LOADFUNC(CLBlastHgemmWithTempBuffer)
    LOADFUNC(CLBlastSGemmTempBufferSize)
    LOADFUNC(CLBlastDGemmTempBufferSize)
    LOADFUNC(CLBlastCGemmTempBufferSize)
    LOADFUNC(CLBlastZGemmTempBufferSize)
    LOADFUNC(CLBlastHGemmTempBufferSize)
    LOADFUNC(CLBlastClearCache)
    LOADFUNC(CLBlastFillCache)
    LOADFUNC(CLBlastOverrideParameters)
    return 0;
}
void rindow_unload_clblast_dll()
{
    FreeLibrary( _h_clblast );
    _h_clblast = NULL;
}
