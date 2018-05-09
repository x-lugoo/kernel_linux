#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <asm/unistd.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/ioctl.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <linux/miscdevice.h>  //attention

#include "brock.h"

#define BROCK_DEV_NAME "brock"


bool canReadFlag;
bool canWriteFlag;


struct wait_queue {
    bool canRead;
    bool canWrite;
    wait_queue_head_t rq;
    wait_queue_head_t wq;
};


struct wait_queue waitQuene;



static int brock_open(struct inode* inodp, struct file* filp) {

    printk("open brock ok!\n");

    return 0;
}

static int brock_release(struct inode* inodp, struct file* filp) {

    return 0;
}


static int brock_ioctl(struct inode* inodp, struct file* filp,
                       unsigned int cmd, unsigned long arg) {
    return 0;
}

static ssize_t brock_read(struct file* filp, char __user* buf, size_t size, loff_t* ppos) {

    int ret = 0;


    if (!canReadFlag) {
        if (filp->f_flags & O_NONBLOCK) {
            printk("no block\n");
            return -EAGAIN;
        }

        printk("read halt----:\n");

        wait_event_interruptible(waitQuene.rq, waitQuene.canRead);
    }

    waitQuene.canRead = false;

    printk("read===========\n");
    canReadFlag = false;


    canWriteFlag = true;
    waitQuene.canWrite = true;
    wake_up(&(waitQuene.wq));

    return ret;
}

static ssize_t brock_write(struct file* filp, const char __user* buf, size_t size, loff_t* ppos) {
    if (!canWriteFlag) {
        if (filp->f_flags & O_NONBLOCK) {
            printk("no block\n");
            return -EAGAIN;
        }

        printk("write halt----:\n");

        wait_event_interruptible(waitQuene.wq, waitQuene.canWrite);
    }

    printk("write=====\n");

    canWriteFlag = false;

    canReadFlag = true;

    waitQuene.canWrite = false;
    waitQuene.canRead = true;
    wake_up(&(waitQuene.rq));


    printk("up quene\n");
    return size;
}


struct file_operations brock_fops = {
    .owner   = THIS_MODULE,
    .open    = brock_open,
    .release = brock_release,
    .read    = brock_read,
    .write   = brock_write,
    .ioctl   = brock_ioctl,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = BROCK_DEV_NAME,
    .fops = &brock_fops,
};


static int __init brock_init(void) {
    int result;

    misc_register(&misc);

    init_waitqueue_head(&(waitQuene.rq));
    init_waitqueue_head(&(waitQuene.wq));
    waitQuene.canRead  = false;
    waitQuene.canWrite = true;

    canReadFlag = false;
    canWriteFlag = true;

    printk(" int brock --->\n");

    return 0;
}

static void brock_exit(void) {
    misc_deregister(&misc);
    printk("cleanup done\n");
}

module_init(brock_init);
module_exit(brock_exit);

MODULE_AUTHOR("lxlong");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("block file test");
MODULE_ALIAS("block");

