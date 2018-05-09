#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/notifier.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>

static RAW_NOTIFIER_HEAD(myNotifier);  /*初始化一个raw通知链*/



struct timer_list timer;

unsigned long count = 0;

void notifier_funtion(unsigned long para) {
    raw_notifier_call_chain(&myNotifier, count , NULL);
    mod_timer(&timer, jiffies + HZ);
    count++;
}

/*3个节点*/
static int notifier_event1(struct notifier_block* this, unsigned long event, void* ptr) {
    printk("call event1: Event param is %d\n", event);
    return 0;
}

static struct notifier_block notifier1 = {
    .notifier_call = notifier_event1,
};

static int notifier_event2(struct notifier_block* this, unsigned long event, void* ptr) {
    printk("call event2: Event param is %d\n", event);
    return 0;
}

static struct notifier_block notifier2 = {
    .notifier_call = notifier_event2,
};
static int notifier_event3(struct notifier_block* this, unsigned long event, void* ptr) {
    printk("call event3: Event param is %d\n", event);
    printk("###############################\n");
    return 0;
}

static struct notifier_block notifier3 = {
    .notifier_call = notifier_event3,
};

int __init myNotifier_init(void) {
    /*注册节点*/
    raw_notifier_chain_register(&myNotifier, &notifier1);
    raw_notifier_chain_register(&myNotifier, &notifier2);
    raw_notifier_chain_register(&myNotifier, &notifier3);



    init_timer(&timer);
    timer.expires = jiffies + HZ;
    timer.function = notifier_funtion;
    add_timer(&timer);
    return 0;
}


void __exit myNotifier_exit(void) {
    del_timer(&timer);
    /*删除节点*/
    raw_notifier_chain_unregister(&myNotifier, &notifier1);
    raw_notifier_chain_unregister(&myNotifier, &notifier2);
    raw_notifier_chain_unregister(&myNotifier, &notifier3);
}

module_init(myNotifier_init);
module_exit(myNotifier_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("lxlong");
MODULE_DESCRIPTION("notifier test");
MODULE_ALIAS("notifier module");
