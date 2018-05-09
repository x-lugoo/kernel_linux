#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lxlong");
MODULE_DESCRIPTION("timer test");
MODULE_ALIAS("timer module");


static int times;
module_param(times, int, S_IRUGO);
MODULE_PARM_DESC(times, "timer set");
struct timer_list timer;


void timer_funtion(unsigned long para) {
    static int count = 1;

    if (count <= para) {
        if (1 == count) {
            printk("<1>\nTimer counter begins:\n");
        }

        printk("<1>count = %d,jiff:%d\n", count, jiffies);
        mod_timer(&timer, jiffies + (HZ * 2));
    } else {
        printk("<1>###");
        printk("<1>timer expired %ld(specified by para) times,\n"
               "and you should remove the module to destroy it\n", para);
    }

    count++;
}

int __init timer_init(void) {
    init_timer(&timer);
    timer.data = times;
    timer.expires = jiffies + HZ;
    timer.function = timer_funtion;
    add_timer(&timer);
    return 0;
}


void __exit timer_exit(void) {
    del_timer(&timer);
    printk("<1>timer has been destroyed\n");
}

module_init(timer_init);
module_exit(timer_exit);
