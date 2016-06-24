#include <linux/sched.h>
#include <linux/net.h>
/*
 * 1. Find kernel objects of one task
 * 2. focus on socket files
 */

// iterate tasks
struct task_struct * find_task_by_pid(pid_t pid);
void list_task_objects(struct task_struct *task);
struct socket *get_one_socket_object(struct task_struct *);

