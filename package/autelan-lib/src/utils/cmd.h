#ifndef __CMD_H_F3687584F159827DAA20B322924194D1__
#define __CMD_H_F3687584F159827DAA20B322924194D1__
/******************************************************************************/

#ifndef __COMMAND_COUNT
#define __COMMAND_COUNT         (2*32)
#endif

#define __COMMAND_F_HIDE        0x01

/*
* ����ģʽ
*  (1): ����֮��û�й�����ϵ�������������
*       os_do_command ��Ҫѭ����������ƥ�������
*       Ŀǰ��֧��
*
*  (2): ����֮���й�����ϵ������������ϣ���Ҫ�г�ȫ����Ч�������
*       os_do_command ֻ�����״�ƥ�䵽������
*       Ŀǰ֧�ֵ�ģʽ
*/

struct command_item {
    /*
    * list����Ԫ��ʵ�ʸ���
    * �����ʼ����do_command �ڲ���ɨ��list�Զ�����
    */
    int count;
    
    /*
    * �����б��������������ʽ
    *   �������� xxx -a address -v value1 value2, ��
    *       list[0] ��ʼ��Ϊ "-a"��     ����ؼ���
    *       list[1] ��ʼ��Ϊ "address"��"-a"�Ĳ���
    *       list[2] ��ʼ��Ϊ "-v"��     ����ؼ���
    *       list[3] ��ʼ��Ϊ "value1"�� "-v"�ĵ�һ������
    *       list[4] ��ʼ��Ϊ "value2"�� "-v"�ĵڶ�������
    *       listʣ��ΪNULL
    *
    * os_do_command ����� list �Զ����ɰ�����Ϣ��Ҫ��
    *   (1)����������һ������(main's argc>1)������޲������Զ����ɲ���ӡ����
    *   (2)ִ�� "xxx -h" ���Զ����ɲ���ӡ��������������ʾ��������
    *   (3)ִ�� "xxx --" ���Զ����ɲ���ӡ������������������
    */
    char *list[__COMMAND_COUNT+1];
    
    /*
    * ����������İ�����Ϣ, ����Ϊ NULL
    */
    char *help;
    
    /*
    * ����ִ�к���
    *   argc, argv �Ǵ� main ���������, argv[0] �ǳ�����
    */
    int (*func)(int argc, char *argv[]);
    
    /*
    * �����ʶ
    *   __COMMAND_F_HIDE ��λ��ʾ����Ϊ��������
    *       ִ�� "xxx" �� "xxx -h" ʱ����ʾ������Ϣ����ʵ�ʿ�ִ��
    *       ִ�� "xxx --" ����ʾ������Ϣ, "xxx --"������help��Ϣ��
    */
    unsigned int flag;

    /*
    * ������Ч�Կ��ƣ��� command_ctrl is_use_mask/bit ���ʹ��
    *
    * ��ʹ��32λ�������粻���ø�Ϊ64λ
    */
    unsigned int mask;
};

struct command_ctrl {
    struct command_item *cmd;   /* array of (struct command_item) */
    int count;                  /* count of cmd         */
    /*
    * ��� is_use_mask Ϊ�٣��� bit �� command��mask ��������
    * ��� is_use_mask Ϊ�棬�� ��ÿ������ִ��������� ((1<<ctrl's bit) & cmd's mask)
    *   ���Խ��Ϊ�棬�����������Ч
    *   ���򣬴���������Ч(�����ɰ���������ִ��)
    * Ŀǰ��Ҫ���ڹ����ض���Ʒ�������
    */
    bool is_use_mask;
    int bit;
};

#define __COMMAND_ARG_HELPS     "-h"
#define __COMMAND_ARG_HELPL     "-help"
#define __COMMAND_ARG_HIDE      "--"

#define __COMMAND_MASK_ALL      0xffffffff

#define __COMMAND_CTRL_INITER(_cmd, _is_use_mask, _bit) { \
    .cmd        = _cmd,             \
    .count      = os_count_of(_cmd),\
    .is_use_mask= _is_use_mask,     \
    .bit        = _bit,             \
}
#define COMMAND_CTRL_INITER(_cmd) \
        __COMMAND_CTRL_INITER(_cmd, false, 0)

static inline struct command_item *
__command_getbyindex(struct command_ctrl *ctrl, int idx)
{
    return &ctrl->cmd[idx];
}


static inline int 
__command_list_count(struct command_item *cmd)
{
    int i;

    for (i=0; i<__COMMAND_COUNT && cmd->list[i]; i++) {
        ;
    }

    return i;
}

static inline void 
__command_init_one(struct command_ctrl *ctrl, int idx)
{
    struct command_item *cmd = __command_getbyindex(ctrl, idx);
    
    /*
    * step:1
    *
    * get command count
    */
    cmd->count = __command_list_count(cmd);

    /*
    * step:2
    *
    * rewrite command mask
    */
    if (ctrl->is_use_mask && 0 == cmd->mask) {
        cmd->mask = __COMMAND_MASK_ALL;
    }
}


static inline void 
__command_init(struct command_ctrl *ctrl)
{
    int i;
    
    for (i=0; i<ctrl->count; i++) {
        __command_init_one(ctrl, i);
    }
}

static inline bool 
__is_command_used(struct command_ctrl *ctrl, int idx)
{
    struct command_item *cmd = __command_getbyindex(ctrl, idx);
    
    return (false == ctrl->is_use_mask || os_hasbit(cmd->mask, ctrl->bit));
}

static inline bool 
__is_command_hide(struct command_item *cmd)
{
    return os_hasflag(cmd->flag, __COMMAND_F_HIDE);
}


static inline void 
__commond_do_help_one(struct command_item *cmd)
{
    int i;

    /* 
    * __tab
    */
    os_printf(__tab);
    
    for (i=0; i<cmd->count && cmd->list[i]; i++) {
        os_printf( "%s ", cmd->list[i]);
    }
    
    /* 
    * help string
    */
    os_printf( __crlf __tab2 "%s" __crlf, cmd->help?cmd->help:"......");
}


static inline void 
__commond_help_one(struct command_ctrl *ctrl, int idx, bool show_hide)
{
    struct command_item *cmd = __command_getbyindex(ctrl, idx);
    
    if (false == __is_command_used(ctrl, idx)) {
        return;
    }
    
    /*
    * is hide command, and not show_hide
    */
    if (__is_command_hide(cmd) && false==show_hide) {
        return;
    }
    
    /*
    * then, make help
    */
    __commond_do_help_one(cmd);
}


static inline void 
__commond_help(struct command_ctrl *ctrl, char *name, bool show_hide)
{
    int i;
    
    /* 
    * help head
    */
    os_println("%s:", name);
    
    for (i=0; i<ctrl->count; i++) {
        __commond_help_one(ctrl, i, show_hide);
    }
}

static inline bool 
__command_match(int argc, char *argv[], struct command_ctrl *ctrl, int idx)
{
    int i;
    struct command_item *cmd = __command_getbyindex(ctrl, idx);

    if ((argc-1)!=cmd->count) {
        return false;
    }
    /*
    * not match current
    */
    else if (false==__is_command_used(ctrl, idx)) {
        return false;
    }
    
    for (i=0; i<cmd->count && cmd->list[i]; i++) {
        if ('-' != cmd->list[i][0]) {
            /* 
            * not begin with '-', need not compare 
            */
            continue;
        }
        
        if (0 != os_strcmp(argv[i+1], cmd->list[i])) {
            /* 
            * main's argv != cmd's arg
            */
            return false;
        }
    }
    
    return (i == cmd->count);
}

/* os_do_command ����

struct command_item commands[] = {
    {
        .list = {"-r", "-a", "address"},    // xxx -r -a address
        .func = reg_read,                   // command function
        .help = "read from the address.",   // command help
    },
    {
        .list = {"-w", "-a", "address", "-v", "value"}, // xxx -w -a address -v value
        .func = reg_write,                      // command function
        .help = "write value to the address.",  // command help
        .flag = __COMMAND_F_HIDE,                 // hide command
    },
};

struct command_ctrl ctrl = __COMMAND_CTRL_INITER(commands, true, 0xffffffff);

os_do_command(argc, argv, &ctrl);

*/
static inline int 
os_do_command(int argc, char *argv[], struct command_ctrl *ctrl)
{
    int i;
    bool show_hide = false;
    
    __command_init(ctrl);

    if (1==argc) {
        goto show_help;
    }
    else if (2 == argc) {
        char *help = argv[1];

        /*
        * "xxx -h"
        * "xxx -help"
        * "xxx --"
        */
        if (0 == os_strcmp(help, __COMMAND_ARG_HELPS)   ||
            0 == os_strcmp(help, __COMMAND_ARG_HELPL)) {
            goto show_help;
        }
        else if (0 == os_strcmp(help, __COMMAND_ARG_HIDE)) {
            show_hide = true;
            goto show_help;
        }
    }

    for (i=0; i<ctrl->count; i++) {
        if (__command_match(argc, argv, ctrl, i)) {
            struct command_item *cmd = __command_getbyindex(ctrl, i);
            
            return (*cmd->func)(argc, argv);
        }
    }
    
show_help:
    __commond_help(ctrl, argv[0], show_hide);
    
    return -EFORMAT;
}

/******************************************************************************/
#endif /* __CMD_H_F3687584F159827DAA20B322924194D1__ */
