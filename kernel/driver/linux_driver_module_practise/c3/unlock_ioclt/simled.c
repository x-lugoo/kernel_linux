#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

#include "simled.h"

#define GPBCON_ADDR  0x56000010

#define GPBDAT_ADDR  0x56000014

u32*     IMM32GPBCON;
u32*           IMM32GPBDAT;


static int led_major = LED_MAJOR;
module_param(led_major, int, S_IRUGO);

struct led_dev* led_devp;
struct cdev cdev;


int led_open(struct inode* inode, struct file* filp) {
    filp->private_data = led_devp;

    return 0;
}

int led_release(struct inode* inode, struct file* filp) {
    return 0;
}

static int led_ioctl(struct file* filp,
                     unsigned int cmd, unsigned long arg) {
    unsigned int temp = 0;

    temp = *IMM32GPBDAT ;

    switch (cmd) {
    case 0:

        temp &= ~(1 << (arg + 5));
        break;

    case 1:
        temp |= 1 << (arg + 5);
        break;

    default:
        break;
    }

    *IMM32GPBDAT = temp;

    return 1;

}
static const struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .open  = led_open,
    .release = led_release,
    .unlocked_ioctl = led_ioctl,
};

static int leddev_init(void) {
    int result;

    dev_t devno = MKDEV(led_major, 0);

    if (led_major) {
        result = register_chrdev_region(devno, 2, "leddev");
    } else {
        result = alloc_chrdev_region(&devno, 0, 2, "leddev");
        led_major = MAJOR(devno);
    }

    if (result < 0) {
        return result;
    }

    IMM32GPBCON = (u32*) ioremap(GPBCON_ADDR, 4);
    IMM32GPBDAT = (u32*) ioremap(GPBDAT_ADDR, 4);

    cdev_init(&cdev, &led_fops);
    cdev.owner = THIS_MODULE;

    cdev_add(&cdev, MKDEV(led_major, 0), 1);

    led_devp = kmalloc(sizeof(struct led_dev), GFP_KERNEL);

    if (!led_devp) {
        result =  - ENOMEM;
        goto fail_malloc;
    }

    memset(led_devp, 0, sizeof(struct led_dev));

    *IMM32GPBCON = 0x15400;
    *IMM32GPBDAT  = 0x0;

    return 0;

fail_malloc:
    unregister_chrdev_region(devno, 1);

    return result;
}

static void leddev_exit(void) {
    cdev_del(&cdev);
    kfree(led_devp);
    unregister_chrdev_region(MKDEV(led_major, 0), 2);
}

MODULE_AUTHOR("lxlong");
MODULE_LICENSE("GPL");


module_init(leddev_init);
module_exit(leddev_exit);
