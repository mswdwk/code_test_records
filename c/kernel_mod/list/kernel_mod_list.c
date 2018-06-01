#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init_task.h>
// 初始化函数
#define format1 "%16s %5s %4s %5s %6s\n"
#define format2 "%16s %5d %4d %5d %6d\n"
static int kernel_mod_list_init(void)
{
    struct task_struct *p;
    p = NULL;
    p = &init_task;
    //printk(KERN_ALERT""fromat1,"名称","进程号","状态","优先级","父进程号");
    printk(format1,"名称","进程号","状态","优先级","父进程号");
    for_each_process(p)
    {
        if(p->mm == NULL){ //内核线程的mm成员为空
            //printk(KERN_ALERT""format2,p->comm,p->pid, p->state,p->normal_prio,p->parent->pid);
            printk(format2,p->comm,p->pid, p->state,p->normal_prio,p->parent->pid);
        }
    }
    return 0;
}
// 清理函数
static void kernel_mod_list_exit(void)
{
    printk(KERN_ALERT"goodbye!\n");
}

// 函数注册
module_init(kernel_mod_list_init);  
module_exit(kernel_mod_list_exit);  

// 模块许可申明
MODULE_LICENSE("GPL");
