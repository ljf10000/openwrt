#ifndef __STRING_H_EBBADBD33FD514F013D3D84007A20302__
#define __STRING_H_EBBADBD33FD514F013D3D84007A20302__
/******************************************************************************/
#ifndef __empty
#define __empty         ""
#endif

#ifndef __space
#define __space         " "
#endif

#ifndef __null
#define __null          "null"
#endif

#ifndef __space2
#define __space2        __space __space
#endif

#ifndef __space4
#define __space4        __space2 __space2
#endif

#ifndef __blanks
#define __blanks        "\t \r\n"
#endif

#ifndef __tab
#ifndef __TAB_AS_SPACE
#define __tab           "\t"
#elif 2==__TAB_AS_SPACE
#define __tab           __space2
#else
#define __tab           __space4
#endif
#endif

#ifndef __tab2
#define __tab2          __tab __tab
#endif

#ifndef __crlf
#define __crlf          "\n"
#endif

#ifndef __crlf2
#define __crlf2         __crlf __crlf
#endif

#ifndef __crlf3
#define __crlf3         __crlf2 __crlf
#endif

#ifndef __crlf4
#define __crlf4         __crlf3 __crlf
#endif

#ifndef __notes
#define __notes         "#"
#endif

#ifndef __unknow
#define __unknow        "unknow"
#endif

#ifndef os_safestring
#define os_safestring(s)            ((s)?(s):__null)
#endif

#ifndef os_strlen
#define os_strlen(s)                strlen(s)
#endif

#ifndef os_strcpy
#define os_strcpy(_dst, _src)       strcpy(_dst, _src)
#endif

/*
* no use strncpy(is unsafe)
*/
static inline size_t 
os_strlcpy(char *dst, const char *src, size_t size)
{
    char *d = (char *)dst;
    char *s = (char *)src;
    int n, len;
    
    os_assert(NULL!=dst);
    os_assert(NULL!=src);

    if (size) {
        n = size - 1;

        while(*s && n) {
            *d++ = *s++;
            n--;
        }

        len = size - 1 - n;
    } else {
        len = 0;
    }

    dst[len] = 0;
    
    return len;
}

#ifndef os_strdcpy
#define os_strdcpy(_dst, _src)      os_strlcpy(_dst, _src, sizeof(_dst))
#endif

#ifndef os_strscpy
#define os_strscpy(_dst, _src)      os_strlcpy(_dst, _src, sizeof(_src))
#endif

#ifndef os_strcmp
#define os_strcmp(_a, _b)           strcmp(_a, _b)
#endif

#ifndef os_strncmp
#define os_strncmp(_a, _b, _n)      strncmp(_a, _b, _n)
#endif

/*
* use a's size
*/
#ifndef os_stracmp
#define os_stracmp(_a, _b)          os_strncmp(_a, _b, sizeof(_a))
#endif

#ifndef os_strtok
#define os_strtok(_s, _delim)       strtok(_s, _delim)
#endif

#ifndef os_strtok_foreach
#define os_strtok_foreach(_sub, _s, _delim) \
        for ((_sub)=os_strtok(_s, _delim);(_sub);(_sub)=os_strtok(NULL, _delim))
#endif

typedef bool char_is_something_f(int ch);

static inline bool 
__char_is(char_is_something_f *IS, int ch)
{
    if (IS) {
        return (*IS)(ch);
    } else {
#ifdef CHAR_IS_DEFAULT
        return CHAR_IS_DEFAULT(ch)
#else
        return NULL!=strchr(__blanks, ch);
#endif
    }
}

/*
* �� string �ڵ� old �滻 Ϊ new
*
* ע�� : string���޸ģ���������
*/
static inline char *
__string_replace(char *string, char_is_something_f *IS_OLD, int new)
{
    char *s = string;

    while(*s) {
        if (__char_is(IS_OLD, *s)) {
            *s++ = new;
        } else {
            s++;
        }
    }

    return string;
}

/*
* �� string �ڵ� ������������������ַ�ȥ��(�������Ϊ1��)
* 
* ע�� : string���޸ģ���������
*/
static inline char *
__string_reduce(char *string, char_is_something_f *IS)
{
    char *p = string; /* ��¼ָ�� */
    char *s = string; /* ɨ��ָ�� */
    bool in_reduce = false; /* reduce ģʽ */
    
    while(*s) {
        if (__char_is(IS, *s)) {
            /*
            * ɨ�赽 ȥ���ַ�, ���¼֮
            *
            * (1) ���� reduce ģʽ
            * (2) p ��������ֻ֤��¼һ�� ȥ���ַ������ﵽ reduce Ч��
            */
            in_reduce = true;
            
            *p = *s++;

        } else {
            /*
            * ɨ�赽��������(�� ȥ���ַ�)
            *
            * �����ǰ�� reduce ģʽ
            *   (1) p ��һ������� reduce��Ϊ��¼����������׼��
            *   (2) �˳� reduce ģʽ
            */
            if (in_reduce) {
                p++;
                
                in_reduce = false;
            }

            /*
            * ��¼��������
            */
            *p++ = *s++;
        }
    }

    *p = 0; /* ����β�� ���� */

    return string;
}

/*
* ���� string ������ IS �� �ַ�
* 
* ע�� : string���޸ģ���������
*/
static inline char *
__string_strim(char *string, char_is_something_f *IS)
{
    char *p = string; /* ��¼ָ�� */
    char *s = string; /* ɨ��ָ�� */

    while(*s) {
        if (__char_is(IS, *s)) {
            s++;
        } else {
            *p++ = *s++;
        }
    }

    return string;
}

/*
* ���� string ������� IS �� �ַ�
* 
* ע�� : string���޸ģ���������
*/
static inline char *
__string_strim_left(char *string, char_is_something_f *IS)
{
    char *p = string; /* ��¼ָ�� */
    char *s = string; /* ɨ��ָ�� */

    /* find first no-match IS */
    while(*s && __char_is(IS, *s)) {
        s++;
    }

    /* all move to begin */
    while(*s) {
        *p++ = *s++;
    }
    
    *p = 0;
    
    return string;
}

/*
* ���� string �Ҳ����� IS �� �ַ�
* 
* ע�� : string���޸ģ���������
*/
static inline char *
__string_strim_right(char *string, char_is_something_f *IS)
{
    /* pointer to last char */
    char *p = string + os_strlen(string) - 1;

    /* scan, from last char to begin */
    while(p>=string && __char_is(IS, *p)) {
        p--;
    }

    /* now, pointer to the right first no-match IS */
    *(p+1) = 0;
    
    return string;
}

static inline char *
__string_strim_both_ends(char *string, char_is_something_f *IS)
{
    __string_strim_right(string, IS);

    return __string_strim_left(string, IS);
}

static inline bool
__char_is_drop(char_is_something_f *IS, int ch)
{
    if (IS) {
        return (*IS)(ch);
    } else {
#ifdef CHAR_IS_DROP
        return CHAR_IS_DROP(ch)
#else
        return ch=='#';
#endif
    }
}

/*
* ���� string �Ҳ����� IS �� �ַ��������ַ�
* 
* ע�� : string���޸ģ���������
*/
static inline char *
__string_r_drop(char *string, char_is_something_f *IS)
{
    /* pointer to last char */
    char *p = string;

    /* scan, from last char to begin */
    while(*p && false==__char_is_drop(IS, *p)) {
        p++;
    }

    *p = 0;
    
    return string;
}

/*
* string long copy
*/
static inline char *
__string_copy(char *dst, char *src, int len)
{
    os_memcpy(dst, src, len);
    dst[len] = 0;

    return dst;
}

static inline bool 
__is_blank_line(char *line)
{
    return 0==line[0] || '\n'==line[0] || ('\r'==line[0] && '\n'==line[1]);
}

static inline bool 
__is_notes_line(char *line, char *notes)
{
    int len = os_strlen(notes);

    return os_memeq(line, notes, len);
}

static inline bool 
__is_notes_line_deft(char *line)
{
    return __is_notes_line(line, __notes);
}
/******************************************************************************/
#endif /* __STRING_H_EBBADBD33FD514F013D3D84007A20302__ */
