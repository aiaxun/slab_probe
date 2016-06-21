#include <linux/sched.h>
#include <linux/net.h>
/*
 * 1. Find kernel objects of one task
 * 2. focus on socket files
 */

// iterate tasks
void all_task(void);

struct task_struct *find_task(pid_t pid);
