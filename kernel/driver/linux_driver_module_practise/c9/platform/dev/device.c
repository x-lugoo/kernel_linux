#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/platform_device.h>

static void my_dev_release(struct device* dev) {

}

//static struct platform_device *my_device;


static struct platform_device my_device = {
    .name         = "s3c",
    .id       = -1,
    .dev        = {
        .release = my_dev_release,

    },
};

static int __init my_device_init(void) {
    int ret = 0;

    /* 分配结构 */
    //  my_device = platform_device_alloc("my_dev", -1);

    /*注册设备*/
    //  ret = platform_device_add(my_device);
    platform_device_register(&my_device);

    /*注册失败，释放相关内存*/
    if (ret) {
        platform_device_put(&my_device);
    }

    return ret;
}

static void my_device_exit(void) {
    platform_device_unregister(&my_device);
}

module_init(my_device_init);
module_exit(my_device_exit);

MODULE_LICENSE("Dual BSD/GPL");

