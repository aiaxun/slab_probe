#include <linux/slub_def.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/fs.h>

void get_task_file_objects(struct task_struct *task);
void desc_object(struct file *file);

void *map_object(struct file *file);
