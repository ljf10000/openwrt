#include "slice/slice.h"
/******************************************************************************/
int slice_vsprintf(struct slice *slice, int flag, const char *fmt, va_list args)
{
    int len = 0;
    va_list arg2;
    
    if (slice) {
        int space;
        
try_again:
        space = slice_remain(slice);
        debug_trace("slice_vsprintf: remain %d", space);
        
        /*
        * ������ slice_remain Ӧ�ô��ڵ��� 0
        *
        * �� space ����Ϊ 1(���ں�����һ����)
        *
        * ����, ��ʣ��ռ�Ϊ1��0ʱ��vsnprintf �������κ�д�����
        */
        space = (space>0)?space:1;

        va_copy(arg2, args);
        len = os_vsnprintf((char *)slice_tail(slice), space, fmt, arg2);
        va_end(arg2);
        debug_trace("slice_vsprintf: needed %d", len);
        
        if (os_snprintf_is_full(space, len)) { /* no space */
            debug_trace("slice_vsprintf: full");
            if (os_hasflag(flag, SLICE_F_GROW) && 0==slice_grow(slice, len + 1 - space)) {
                debug_trace("slice_vsprintf: grow and try");
                
                goto try_again;
            } else {
                /* do nothing */
            }
        } else {
            slice_put(slice, len);
        }
    } 
    else {
        char tmp[4];
        
        /*
        * ����ֻ�Ǽ�����Ҫ���ٿռ�
        */
        len = vsnprintf(tmp, 0, fmt, args);
    }
    
    return len;
}

int slice_sprintf(struct slice *slice, int flag, const char *fmt, ...)
{
    int len = 0;
    va_list args;

    va_start(args, fmt);
    len = slice_vsprintf(slice, flag, fmt, args);
    va_end(args);
        
    return len;
}

/******************************************************************************/
AKID_DEBUGER; /* must last os_constructor */

