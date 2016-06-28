#include <linux/sched.h>
#include <linux/net.h>
#include <net/sock.h>

/*
 * 1. Find kernel objects of one task
 * 2. focus on socket files
 */

// iterate tasks
struct task_struct * find_task_by_pid(pid_t pid);
void list_task_objects(struct task_struct *task);
struct socket *get_one_socket_object(struct task_struct *);

int count_task_socket_objects(struct task_struct *task);
void analysis_socket(struct socket *s);
