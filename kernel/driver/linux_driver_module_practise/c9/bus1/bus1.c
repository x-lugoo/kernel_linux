#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>

static char* Version = "$Revision: 1.9 $";

static int my_match(struct device* dev, struct device_driver* driver) {
    return !strncmp(dev_name(dev), driver->name, strlen(driver->name));
}



struct bus_type my_bus_type_new = {
    .name = "my_bus_new",
    .match = my_match,
};

EXPORT_SYMBOL(my_bus_type_new);


/*
 * Export a simple attribute.
 */
static ssize_t show_bus_version(struct bus_type* bus, char* buf) {
    return snprintf(buf, PAGE_SIZE, "%s\n", Version);
}

static BUS_ATTR(version, S_IRUGO, show_bus_version, NULL);


static int __init my_bus_init(void) {
    int ret;

    /*注册总线*/
    ret = bus_register(&my_bus_type_new);

    if (ret) {
        return ret;
    }

    /*创建属性文件*/
    if (bus_create_file(&my_bus_type_new, &bus_attr_version)) {
        printk(KERN_NOTICE "new Fail to create version attribute!\n");
    }

    return ret;
}

static void my_bus_exit(void) {
    bus_unregister(&my_bus_type_new);
}

module_init(my_bus_init);
module_exit(my_bus_exit);

MODULE_LICENSE("Dual BSD/GPL");
