#ifndef _MY_DEV_H
#define _MY_DEV_H
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <asm/uaccess.h>

#define MYCDEV_SIZE 128

#define ATTACH_TASK 1
#define DUMP_OBJECT 3 
#define MMAP_KPAGES 5
#define DUMP_STACK  7
#define COUNT_OBJECT 9


//int mycdev_open(struct inode *inode, struct file *fp);
//int mycdev_release(struct inode *inode, struct file *fp);
//ssize_t mycdev_read(struct file *fp, char __user *buf, 
//        size_t size, loff_t *pos);

//static ssize_t mycdev_write(struct file *fp, const char __user *buf, 
//        size_t size, loff_t *pos);

int mydev_init(void);
void mydev_exit(void);

#endif
