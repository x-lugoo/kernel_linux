#include <linux/kthread.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/cpumask.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/freezer.h>
#include <linux/cpu.h>
#include <linux/clockchips.h>
#include <linux/slab.h>


#ifndef SLEEP_MILLI_SEC
#define SLEEP_MILLI_SEC(nMilliSec)\
    do { \
        long timeout = (nMilliSec) * HZ / 1000; \
        while(timeout > 0) \
        { \
            timeout = schedule_timeout(timeout); \
        } \
    }while(0);
#endif

static struct task_struct* MyThread = NULL;
static int MyPrintk(void* data) {
    char* mydata = kmalloc(strlen(data) + 1, GFP_KERNEL);
    memset(mydata, '\0', strlen(data) + 1);
    strncpy(mydata, data, strlen(data));

    while (!kthread_should_stop()) {
        SLEEP_MILLI_SEC(1000);
        printk("%s\n", mydata);
    }

    kfree(mydata);
    return 0;
}
static int __init init_kthread(void) {
    MyThread = kthread_run(MyPrintk, "hello world", "mythread");
    return 0;
}
static void __exit exit_kthread(void) {
   printk("mythread point=%08X\n",MyThread);
    if (MyThread) {
        printk("stop MyThread\n");
        kthread_stop(MyThread);
    }
}
module_init(init_kthread);
module_exit(exit_kthread);
MODULE_AUTHOR("lxlong");
MODULE_LICENSE("GPL");
