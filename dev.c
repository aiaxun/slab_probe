#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include "dev.h"
#include "task.h"

static struct task_struct *task = NULL;
static struct task_struct *target = NULL;
static int readed = 0;
static int mycdev_open(struct inode *inode, struct file *fp)
{
    if (!task) {
        task = current;
        printk("task %s is using this module\n", task->comm);
    } else {
        printk("module is busy now\n");
        return -1;
    }
	return 0;
}

static struct task_struct *find_task(pid_t pid) 
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

static int mycdev_release(struct inode *inode, struct file *fp)
{

    if (task) {
        printk("task %s release this module\n", task->comm);
        task = NULL;
    }
	return 0;
}

static ssize_t mycdev_read(struct file *fp, char __user *buf, size_t size, loff_t *pos)
{
	unsigned long p = *pos;
	unsigned int count = size;
	int i;
	char kernel_buf[MYCDEV_SIZE] = "This is mycdev!\0\0";

	if(p >= MYCDEV_SIZE)
		return -1;
	if(count > MYCDEV_SIZE)
		count = MYCDEV_SIZE - p;

	if (copy_to_user(buf, kernel_buf, count) != 0) {
		printk("read error!\n");
		return -1;
	} 
    if (readed==0) {
        readed++;
    }else {
        return -1;
    }

	/*
	for (i = 0; i < count; i++) {
		__put_user(i, buf);//write 'i' from kernel space to user space's buf;
		buf++
	}
	*/

	printk("edsionte's reader: %d bytes was read...\n", count);
	return count;

}

static int mycdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct inode *inode = file_inode(file);
    int ret = 0;
    printk("cmd = %u, arg = %lu\n", cmd, arg);
    
    /*
     * ATTACH_TASK: 关联到某进程上
     * DUMP_OBJECT: 把对象dump出来
     * MMAP_KPAGES: 映射页面到用户空间
     * DUMP_STACK : Copy 进程的内核栈到用户空间
     */
    switch(cmd) {
        case ATTACH_TASK:
            printk("attaching task pid:%d\n",arg);
            target = find_task( (pid_t) arg);
            if (!target) {
                printk("do not find task\n");
                ret = -1;
                break;
            }
           list_task_objects(target); 
            
        break;
        case DUMP_OBJECT:
        break;
        case MMAP_KPAGES:
        break;
        case DUMP_STACK:
        break;
        defult:
        break;
    }

    return ret;
}

static ssize_t mycdev_write(struct file *fp, const char __user *buf, size_t size, loff_t *pos)
{
	return size;
}

/*filling the mycdev's file operation interface in the struct file_operations*/
/*
static const struct file_operations mycdev_fops =
{
	.owner = THIS_MODULE,
	.read = mycdev_read,
	.write = mycdev_write,
	.open = mycdev_open,
	.release = mycdev_release,
};
*/

static const struct file_operations mycdev_fops = {
    .owner = THIS_MODULE,
    .read = mycdev_read, 
    .write = mycdev_write, 
    .open = mycdev_open,
    .unlocked_ioctl = mycdev_ioctl,
    .release = mycdev_release,
};


static struct miscdevice mydev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "kernel_obj",
    .fops = &mycdev_fops
};

/*module loading function*/
int mydev_init(void)
{
	int ret;

	printk("mycdev module is staring..\n");

	//ret=register_chrdev(MYCDEV_MAJOR,"slab_debug", &mycdev_fops);
	ret = misc_register(&mydev);
    if(ret<0)
	{
		printk("register failed..\n");
		return 0;
	}
	else
	{
		printk("register success..\n");
	}

	return 0;
}

void mydev_exit(void)
{
	printk("mycdev module is leaving..\n");
	//unregister_chrdev(MYCDEV_MAJOR, "slab_debug");
    misc_deregister(&mydev);
}


