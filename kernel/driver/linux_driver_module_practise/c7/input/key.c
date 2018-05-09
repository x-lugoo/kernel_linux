#include <linux/module.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/input.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/io.h>
#include "key.h"

#define KEY_DEVICE_NAME "key"


struct timer_list timer;
static void __iomem* IMM32;
struct input_dev* dev;

void timer_funtion(unsigned long para) {
    int data;

    data = readl(IMM32 + GPNDAT_OFFSET);

    if (data & 0x1) { //key1
        input_report_key(dev, BTN_0, 1);

    } else {
        input_report_key(dev, BTN_0, 0);
    }

    if (data & 0x8) { //key2
        input_report_key(dev, BTN_3, 1);

    } else {
        input_report_key(dev, BTN_3, 0);
    }

    input_sync(dev);

    mod_timer(&timer, jiffies);
}


static int input_open(struct input_dev* dev) {

    printk("open input\n");
    return 0;
}

static void input_close(struct input_dev* dev) {
    return;
}



static int __init dev_init(void) {
    int err;

    IMM32 = ioremap(ADDRESS, 8);
    dev = input_allocate_device();

    if (dev == NULL) {
        printk(KERN_ERR "Unable to allocate the input device !!/n");
        return -ENOMEM;
    }

    //    dev->evbit[0] = BIT_MASK(EV_KEY);
    //    dev->evbit[1] = BIT_MASK(EV_KEY);
    set_bit(EV_KEY, dev->evbit);
    //    set_bit(EV_SYN, dev->evbit);
    set_bit(BTN_0, dev->keybit);
    set_bit(BTN_3, dev->keybit);

    dev->name = "lxlInput";
    dev->id.bustype = BUS_RS232;
    dev->id.vendor = 0x0001;
    dev->id.product = 0x0001;
    dev->id.version = 0x0100;
    dev->phys = "key";
    dev->dev.init_name = "yiyi_input";
    dev->open = input_open;
    dev->close = input_close;


    err = input_register_device(dev);

    if (err) {
        printk(KERN_ERR "failed to register input device/n");
        input_free_device(dev);
    }

    printk("initialized\n");

    init_timer(&timer);
    timer.expires = jiffies + HZ;
    timer.function = timer_funtion;
    add_timer(&timer);

    return err;
}


static void __exit dev_exit(void) {
    input_unregister_device(dev);
    del_timer(&timer);
}


module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("YiYi");

