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


#define for_each_object(__p, __s, __addr, __objects) \
        for (__p = (__addr); __p < (__addr) + (__objects) * (__s)->size;\
             __p += (__s)->size)

//extern static struct list_head *slab_caches;
void get_task_file_objects(struct task_struct *task);
void desc_object(struct file *file);

void *map_object(struct file *file);


static inline int slab_index(void *p, struct kmem_cache *s, void *addr)
{
    return (p - addr)/s->size;
}

static inline void *get_freepointer(struct kmem_cache *s, void *object)
{
    return *(void **)(object + s->offset);
}

void get_map(struct kmem_cache *s, struct page *page, unsigned long *map);
void list_slab_objects(struct kmem_cache *s, struct page *page);

struct list_head *get_slab_head(void);
