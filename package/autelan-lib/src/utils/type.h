#ifndef __TYPE_H_6BA367A7D4B17C3588DAABDD4B9B396D__
#define __TYPE_H_6BA367A7D4B17C3588DAABDD4B9B396D__
/******************************************************************************/
#ifndef NULL
#define NULL            ((void *)0)
#endif

#ifndef BOOL
#define BOOL            int
#endif

#ifndef bool
#define bool            BOOL
#endif

#ifndef TRUE
#define TRUE            1
#endif

#ifndef true
#define true            TRUE
#endif

#ifndef FALSE
#define FALSE           0
#endif

#ifndef false
#define false           FALSE
#endif

#ifndef BYTE
#define BYTE            unsigned char
#endif

#ifndef byte
#define byte            BYTE
#endif

#if defined(LP64) || defined(ILP64) || defined(LLP64)
#define OS64            1
#else
#define OS64            0
#endif

#define OS_ENABLE       1
#define OS_DISABLE      0

#define OS_ON           1
#define OS_OFF          0

/*
* ѭ��ָʾ��, ����ָ���Ƿ����ѭ������
*/
#define OS_MV_GO        0
#define OS_MV_BREAK     1

typedef int multi_value_t;

/*
* ��ֵ: ���ں������ض�ֵ, ��Ҫ˫��Լ����ֵ��ʽ
*
* ���õ��� mv2 ϵ��, ��Ҫ���ڻص�����
*   mv2_result(mv)      ���ڷ��ش���ֵ
*   mv2_control(mv)     ���ڷ���ѭ��ָʾ�� OS_MV_GO/OS_MV_BREAK
*
*   mv2_BREAK(_result)
*       ���ڷ��ش���ֵ + OS_MV_BREAK, ��ʾ�д���, ��Ҫ��ֹѭ��
*
*   mv2_GO(_result)
*       ���ڷ��ش���ֵ + OS_MV_GO, ��ʾ�д��󵫿��Ժ��ԣ�����ѭ������
*
*   mv2_OK
*       ���ڷ��سɹ�   + OS_MV_GO, ��ʾִ�гɹ�������ѭ������
*/
typedef union {
    multi_value_t value;
    
    struct {
        int result:24;
        int control:8;
    } v2;
    
    struct {
        int result:16;
        int control:8;
        int private:8;
    } v3;
    
    struct {
        int result:8;
        int control:8;
        int private:8;
        int value:8;
    } v4;
}
multi_value_u;

#define MV_INITER               { .value = 0 }

#define mv2_result(mv)          (mv).v2.result
#define mv2_control(mv)         (mv).v2.control
#define mv2_INITER(_control, _result) { \
    .v2 = {                             \
        .result = _result,              \
        .control= _control,             \
    }                                   \
}

static inline multi_value_t 
__mv2_return(int control, int result)
{
    multi_value_u mv = mv2_INITER(control, result);

    return mv.value;
}

#define mv2_BREAK(_result)      __mv2_return(OS_MV_BREAK, _result)
#define mv2_GO(_result)         __mv2_return(OS_MV_GO, _result)
#define mv2_OK                  0 /* mv2_GO(0) */

#define mv2_is_break(mv)        (OS_MV_BREAK==mv2_control(mv))
#define mv2_is_go(mv)           (OS_MV_GO==mv2_control(mv))

#define mv3_result(mv)          (mv).v3.result
#define mv3_control(mv)         (mv).v3.control
#define mv3_private(mv)         (mv).v3.private

#define mv4_result(mv)          (mv).v4.result
#define mv4_control(mv)         (mv).v4.control
#define mv4_private(mv)         (mv).v4.private
#define mv4_value(mv)           (mv).v4.value
/******************************************************************************/
#endif /* __TYPE_H_6BA367A7D4B17C3588DAABDD4B9B396D__ */
