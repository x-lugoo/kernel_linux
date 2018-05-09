#include <linux/module.h>
#include <linux/kernel.h>


static int __init my_driver_init(void) {
    printk(" hello --->\n");

    return 0;
}

static void my_driver_exit(void) {
    printk("byebye\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_AUTHOR("lxlong");
MODULE_LICENSE("GPL");


