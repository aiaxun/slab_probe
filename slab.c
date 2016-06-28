#include "slab.h"
#include <linux/skbuff.h>
static struct list_head *slabs;


void get_map(struct kmem_cache *s, struct page *page, unsigned long *map)
{
    void *p;
    void *addr = page_address(page);
    for (p = page->freelist; p; p = get_freepointer(s, p)) 
    {
        set_bit(slab_index(p, s, addr), map);
    }
}
void list_slab_objects(struct kmem_cache *s, struct page *page)
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

struct list_head *get_slab_head(void)
{
    unsigned long addr = kallsyms_lookup_name("slab_caches");
    if (addr) {
        struct list_head *slab = (struct list_head *)addr;
        return slab;
    } else {
        return NULL;
    }
}
