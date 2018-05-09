#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/poll.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include "poll.h"
#include <linux/miscdevice.h>  //attention

#define DEVICE_NAME "polldev"

struct poll_dev dev;

void timer_function(unsigned long para) {
    wake_up_interruptible(&dev.wq);
    dev.flag = true;
    mod_timer(&dev.timer, jiffies +  HZ);
}



int poll_open(struct inode* inode, struct file* filp) {
    filp->private_data = &dev;

    printk("open\n");
    return 0;
}

int poll_release(struct inode* inode, struct file* filp) {
    struct poll_dev* devpoll = filp->private_data;
    return 0;
}

static ssize_t poll_read(struct file* filp, char __user* buf, size_t size, loff_t* ppos) {
    unsigned int count;
    int ret = 0;
    struct poll_dev* devpoll = filp->private_data; /*获得设备结构体指针*/

    return ret;
}

static ssize_t poll_write(struct file* filp, const char __user* buf, size_t size, loff_t* ppos) {
    struct poll_dev* devpoll = filp->private_data; /*获得设备结构体指针*/

    return size;
}

static unsigned int poll_poll(struct file* filp, poll_table* wait) {
    struct poll_dev* devpoll = filp->private_data;
    unsigned int mask = 0;
    poll_wait(filp, &dev.wq, wait);
    mask |= POLLIN | POLLRDNORM;

    wait_event_interruptible(dev.wq, dev.flag);
    dev.flag = false;
    return mask;
}
static const struct file_operations poll_fops = {
    .owner = THIS_MODULE,
    .read = poll_read,
    .write = poll_write,
    .poll = poll_poll,
    .open = poll_open,
    .release = poll_release,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &poll_fops,

};


int poll_init(void) {
    misc_register(&misc);

    init_timer(&dev.timer);
    dev.timer.expires = jiffies + HZ;
    dev.timer.function = timer_function;
    add_timer(&dev.timer);
    dev.flag = false;

    init_waitqueue_head(&dev.wq);
    printk(DEVICE_NAME"\tinitialized\n");
    return 0;
}

void poll_exit(void) {
    misc_deregister(&misc);
    del_timer(&dev.timer);
    printk("cleanup done\n");
}


MODULE_AUTHOR("lxlong");
MODULE_LICENSE("GPL");

module_init(poll_init);
module_exit(poll_exit);
