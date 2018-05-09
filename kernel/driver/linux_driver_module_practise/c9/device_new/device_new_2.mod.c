#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x59caa4c3, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xe98f354, __VMLINUX_SYMBOL_STR(my_bus_type_new) },
	{ 0x2c514473, __VMLINUX_SYMBOL_STR(my_dev) },
	{ 0xf6d20ec1, __VMLINUX_SYMBOL_STR(device_unregister) },
	{ 0x19998f84, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0x24f9a0a1, __VMLINUX_SYMBOL_STR(device_register) },
	{ 0xc7eb2e31, __VMLINUX_SYMBOL_STR(dev_set_name) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=bus1,device";


MODULE_INFO(srcversion, "D54BB4B35D5003D9AE0363E");
