/*************************************************************************
	> File Name: test.c
	> Author: sqt
	> Mail: shenqintao@pwrd.com
	> Created Time: 2016年04月18日 星期一 10:23:02
 ************************************************************************/

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/string.h>
#include <linux/slub_def.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/bitops.h>
#include <linux/bit_spinlock.h>
#include <linux/sched.h>
#include <linux/socket.h>
#include <linux/fdtable.h>
#include <linux/fs.h>
#include <linux/net.h>
#include <linux/path.h>
#include <linux/dcache.h>
#include "dev.h"
/*
 * 主要关注task_struct的socket文件对象 
 */

void all_task(void)
{
    struct task_struct *p;
    int i;
    struct file *f;
    struct fdtable *fdt;
    struct socket *sock;
    int err;
    struct path *f_path;
    struct dentry *dentry;
    char buf[255], *path;
    //unsigned long *socket_file_ops = kallsyms_lookup_name("socket_file_ops");
    printk("PID\tNAME\tF_PID\tF_NAME\tmax_fds\n");
    for_each_process(p) {
        printk("*****************************\n");
        printk("%d\t%s\t%d\t%s\t", p->pid, p->comm, p->parent->pid, p->parent->comm);
        printk("%d\nfdtable\t", p->files->fdt->max_fds);
        fdt = p->files->fdt;
        for (i=0;i<fdt->max_fds; i++)
        {
            f = fdt->fd[i];
            if (!f) {
                break;
            }
            sock = sock_from_file(f, &err);
            if (sock ) {
                f_path = &f->f_path;
                dentry = f_path->dentry;
                path = d_path(f_path, buf, 255);
                printk("find socket %p\tfamily:%d\tname:%s\t", sock, sock->ops->family, path);
                printk("%s, %s\t", dentry->d_iname, dentry->d_parent->d_iname);
            }
            /*
            }*/
        }

        printk("\n\n");

    }
    /*
     * files list in one task:
     *      task->files->fd_array[]
     */


}
static struct list_head *slabs;


#define for_each_object(__p, __s, __addr, __objects) \
        for (__p = (__addr); __p < (__addr) + (__objects) * (__s)->size;\
             __p += (__s)->size)

static inline int slab_index(void *p, struct kmem_cache *s, void *addr)
{
    return (p - addr)/s->size;
}

static inline void *get_freepointer(struct kmem_cache *s, void *object)
{
    return *(void **)(object + s->offset);
}

static void get_map(struct kmem_cache *s, struct page *page, unsigned long *map)
{
    void *p;
    void *addr = page_address(page);
    for (p = page->freelist; p; p = get_freepointer(s, p)) 
    {
        set_bit(slab_index(p, s, addr), map);
    }
}
static void list_slab_objects(struct kmem_cache *s, struct page *page)
{
#ifdef CONFIG_SLUB_DEBUG
    void *addr = page_address(page);
    void *p;
    int u, n, i;
    unsigned long *map = kzalloc(BITS_TO_LONGS(page->objects) * sizeof(long), GFP_ATOMIC);
    if (!map)
        return;
    //slab_lock(page);
    bit_spin_lock(PG_locked, &page->flags);
    get_map(s, page, map);
    printk("\n-----------\n");
    u = 0;
    n = 0;
    for_each_object(p, s, addr, page->objects) {
        i = slab_index(p , s, addr);
        printk("%d ", i);
        if (!test_bit(slab_index(p, s, addr), map)) {
            u++;
            printk("addr:%4x: %4x %4x %4x %4x\n", p, *(int *)p, *(int *)(p+1), *(int *)(p+2), *(int *)(p+3), *(int*)(p+4));
        }
        n++;
    }
    printk("used:%d, total:%d\n", u, n);
    //slab_unlock(page);
    __bit_spin_unlock(PG_locked, &page->flags);
    kfree(map);
#endif
}
static int __init test_init(void)
{
    /*
    unsigned long addr = kallsyms_lookup_name("slab_caches");
    struct list_head *slab_caches = (struct list_head *)addr;
    struct list_head *p;
    struct kmem_cache *tmp;
    struct page *page, *ppage, *t;
    struct page *partial;
    void **freelist;
    struct kmem_cache_cpu *c;
    int cpu;
    slabs = slab_caches;
    list_for_each(p ,slab_caches) {
        if (p != slab_caches) {
            tmp = list_entry(p , struct kmem_cache, list);
            printk("name:%s\n", tmp->name);
            printk("size: %d\tobj_size:%d\n", tmp->size, tmp->object_size);
            cpu = 0;
            for_each_possible_cpu(cpu) {
                c = per_cpu_ptr(tmp->cpu_slab, cpu);
                if (c->page || c->partial)
                {
                    if (c->page) {
                        page = c->page;
                        printk("local page %p, objects:%d, inuse: %d\n", c->page, c->page->objects, c->page->inuse);
                        list_slab_objects(tmp, c->page);
                    }
                    if (c->partial) {
    
                        printk("locatl partial %p, objects: %d, inuse: %d\n", c->partial, 
                            c->partial->objects, c->partial->inuse);
                        list_slab_objects(tmp, c->partial);
                    }
                }
            }
            printk("******************************************\n\n"); 
            
        }
    }
    all_task();*/
    printk("++++++++++++++++++++++++++++++++\n");
    mydev_init();
    return 0;
}

static void  __exit test_exit(void)
{
    mydev_exit();
    printk("bye\n");
}

MODULE_LICENSE("GPL");
module_init(test_init);
module_exit(test_exit);
