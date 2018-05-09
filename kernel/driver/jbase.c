#include<linux/module.h>
#include<linux/init.h>
#include<linux/device.h>


struct device_driver test_driver;
struct device test_device;
struct bus_type test_bus_type;
static int test_device_match(struct device *dev,struct device_driver *drv)
{
	printk(KERN_INFO "jbase driver is %s,%s(%d)",drv->name,__func__,__LINE__);
	
	if(drv == &test_driver && dev == &test_device){
		printk(KERN_INFO "jbase match\n");
		return 1;
	}else{
		printk(KERN_INFO "jbase no match\n");
		return 0;
	}
	return 0;
}

int test_dev_probe(struct device *dev)
{
	printk("tbase: Entered test_dev_probe\n");
	return 0;
}

int test_dev_remove(struct device *dev)
{
	printk("tbase: Entered test_dev_remove\n");
	return 0;
}

struct device_driver test_driver = {
	.name = "TestDriver",
	.bus = &test_bus_type,
	.probe = test_dev_probe,
	.remove = test_dev_remove,
};

struct device test_device = {
	// .name = "TestDevice",
	.bus = &test_bus_type,
//	.bus_id = "test_bus",
};
		

struct bus_type test_bus_type = {
	.name = "test_bus",
	.match = test_device_match,
};

static int  __init jbase_init_module(void)
{
	printk(KERN_INFO "init_module\n");
	bus_register(&test_bus_type);
	driver_register(&test_driver);
	device_register(&test_device);
	return 0;
}

static void __exit jbase_exit_module(void)
{
	printk(KERN_INFO "exit_module\n");
	device_unregister(&test_device);
	driver_unregister(&test_driver);
	bus_unregister(&test_bus_type);

}


module_init(jbase_init_module);
module_exit(jbase_exit_module);
MODULE_LICENSE("GPL");
