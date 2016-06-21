#include "task.h"

void all_task(void)
{
    struct task_struct *p;
    for_each_process(p) {
        printk("%d\t%s\n", p->pid, p->comm);
    }
}
