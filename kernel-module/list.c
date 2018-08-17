/*
   08/16/2018
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/sched/signal.h> 
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/udp.h>
#include <linux/mm.h>
#include <linux/kthread.h>

unsigned int quit = 0U;
struct task_struct *thread;

static int run_it(void *data) {
  static unsigned int x = 0U;
  static unsigned int idx = 0U;
  static unsigned int skip = 0U;
  static unsigned int i_arr[1000] = {0U};
  static char c_arr[1000][256] = {""};

  while (1) {
    struct task_struct *task = NULL;
    if (1U == quit) {
      break;
    }
    for_each_process(task) {
      if (idx >= 999U) {
        memset(i_arr, 0, sizeof(i_arr));
        memset(c_arr, 0, sizeof(c_arr));
        idx = 0U;
      }
      for (x = 0U; x <= idx; x++) {
        if (i_arr[x] == (unsigned int)(task->pid) && 0 == (strcmp(task->comm, c_arr[x]))) {
          skip = 1U;
          break;
        }
      }
      if (0 == (strcmp(task->comm, ""))) {
        skip = 1U;
      }
      if (1U != skip) {
        i_arr[idx] = (unsigned int)task->pid;
        snprintf(c_arr[idx], 255, "%s", task->comm);

        printk("%d %s\n", task->pid, task->comm);
        idx++;
      }
      skip = 0U;
      schedule();
    }
    ssleep(5);
  }
  do_exit(0);
  return 0;
}

static int __init init_hello(void) {
  thread = kthread_run(run_it, NULL, "list processes");
  if (thread) {
    wake_up_process(thread);
  }
  return 0;
}

static void __exit init_exit(void) {
  quit = 1U;
  kthread_stop(thread);
}

module_init(init_hello);
module_exit(init_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("su8");
MODULE_DESCRIPTION("List all processes and log them to dmesg");
