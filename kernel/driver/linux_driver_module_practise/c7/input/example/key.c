#include <linux/input.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/irq.h>
#include <asm/io.h>

static struct input_dev* button_dev;

static int __init button_init(void) {
    int error;

    button_dev = input_allocate_device();

    if (!button_dev) {
        printk(KERN_ERR "button.c: Not enough memory\n");
        error = -ENOMEM;
        goto err_free_dev;
    }

    button_dev->evbit[0] = BIT_MASK(EV_KEY);
    button_dev->keybit[BIT_WORD(BTN_0)] = BIT_MASK(BTN_0);
    error = input_register_device(button_dev);

    if (error) {
        printk(KERN_ERR "button.c: Failed to register device\n");
        goto err_free_dev;
    }

    return 0;
err_free_dev:
    input_free_device(button_dev);
    return error;
}
static void __exit button_exit(void) {
    input_unregister_device(button_dev);
}
module_init(button_init);
module_exit(button_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("lxlong");
