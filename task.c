#include "task.h"
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/net.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/socket.h>
#include <linux/dcache.h>
#include <linux/fs.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/slub_def.h>
#include <linux/slab.h>
#include <linux/fdtable.h>
#include <linux/path.h>


struct task_struct *find_task_by_pid(pid_t pid)
{
    struct task_struct *p;
    for_each_process(p)
    {
        if (p->pid == pid) {
            return p;
        }
    }
    return NULL;
}

void list_task_objects(struct task_struct *task)
{
    struct file *tmp_file;
    struct fdtable *fdt;
    struct socket *socket;
    int err;
    int i;
    struct path *f_path;
    struct dentry *dentry;
    char buf[255], *path;

    if (!task) {
        printk("task pointer in NULL\n");
        return ;
    }
    printk("%d\t%s\n", task->pid, task->comm);
    fdt = task->files->fdt;
    for (i=0;i<fdt->max_fds; i++)
    {
        tmp_file = fdt->fd[i];
        if (!tmp_file) {
            break;
        }
        socket = sock_from_file(tmp_file, &err);
        if (socket) {
            f_path = &tmp_file->f_path;
            dentry = f_path->dentry;
            path = d_path(f_path, buf, 255);
            printk("find socket %p\tname:[%s]\t",socket, path);
            printk("%s, %s\t", dentry->d_iname, dentry->d_parent->d_iname);
            printk("\n\n");
        }
    }
    
}

