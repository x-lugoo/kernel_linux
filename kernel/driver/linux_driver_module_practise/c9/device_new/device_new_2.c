#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>

extern struct device my_dev;
extern struct bus_type my_bus_type_new;

/* Why need this ?*/
static void my_dev_release(struct device* dev) {

}

struct device my_dev_new = {
    .bus = &my_bus_type_new,
    .parent = &my_dev,
    .release = my_dev_release,
};

/*
 * Export a simple attribute.
 */
static ssize_t mydev_show(struct device* dev, char* buf) {
    return sprintf(buf, "%s\n", "This is my device!");
}

static DEVICE_ATTR(dev_new, S_IRUGO, mydev_show, NULL);

static int __init my_device_init(void) {
    int ret = 0;

    /* 初始化设备 */
    dev_set_name(&my_dev_new, "my_dev_new");


    /*注册设备*/
    device_register(&my_dev_new);

    /*创建属性文件*/
    device_create_file(&my_dev_new, &dev_attr_dev_new);

    return ret;

}

static void my_device_exit(void) {
    device_unregister(&my_dev_new);
}

module_init(my_device_init);
module_exit(my_device_exit);

MODULE_LICENSE("Dual BSD/GPL");
