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
#include <linux/skbuff.h>

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
    //printk("%d\t%s\n", task->pid, task->comm);
    fdt = task->files->fdt;
    for (i=0;i<fdt->max_fds; i++)
    {
        tmp_file = fdt->fd[i];
        if (!tmp_file) {
            continue;
        }
        socket = sock_from_file(tmp_file, &err);
        if (socket) {
            f_path = &tmp_file->f_path;
            dentry = f_path->dentry;
            path = d_path(f_path, buf, 255);
            //printk("find socket %p\tname:[%s]\t",socket, path);
            //printk("%s, %s\t", dentry->d_iname, dentry->d_parent->d_iname);
            //printk("\n\n");
        }
    }
    
}
struct socket *get_one_socket_object(struct task_struct *task)
{
    struct fdtable *fdt;
    struct file *f;
    int i,err;
    struct socket *s;
    if (!task) { return NULL; }
    fdt = task->files->fdt;
    for (i=0;i<fdt->max_fds;i++)
    {
        f = fdt->fd[i];
        if (!f) continue;
        s = sock_from_file(f, &err);
        if (s) {
            printk("find socket object \n");
            return s;
        }
    }
    return NULL;
}

int count_task_socket_objects(struct task_struct *task)
{
    int counter = 0;
    int i, err;
    struct socket *s;
    struct fdtable *fdt;
    struct file *f;

    if (!task) return counter;
    fdt = task->files->fdt;

    for (i=0;i<fdt->max_fds;i++)
    {
        f = fdt->fd[i];
        if (!f) continue;
        s = sock_from_file(f, &err);
        if (s) {
            counter ++;
        }
    }
    return counter;
}

void analysis_socket(struct socket *socket)
{
    if (!socket) return ;
    struct sock *sock = socket->sk;
    struct sk_buff *skb;
    int i;
    if (!sock) return;
    spin_lock(&sock->sk_lock.slock);
    printk("sock->sk_rcvbuf:    %d\n", sock->sk_rcvbuf);
    printk("receive queue:      %p, %lu\n", sock->sk_receive_queue, sock->sk_receive_queue.qlen);
    printk("sock->sk_sndbuf:    %d\n", sock->sk_sndbuf);
    printk("sk_backlog->len:    %u\n", sock->sk_backlog.len);
    printk("sk user data:       %x\n", sock->sk_user_data);
    skb = (struct sk_buff *)sock->sk_receive_queue.next;
    if (skb) {
        printk("len %d, data_len %d, true size: %u\n", skb->len, skb->data_len, skb->truesize);
        printk("data %p\n", skb->data);
        for (i=0;i<skb->truesize;i++)
        {
            if (i%16==0) printk("\n");
            printk("%lu ", skb->data[i]);
        }

    }

    spin_unlock(&sock->sk_lock.slock);
}
