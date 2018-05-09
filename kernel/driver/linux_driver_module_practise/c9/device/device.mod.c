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
	{ 0x686b4c9a, __VMLINUX_SYMBOL_STR(my_bus) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x24f9a0a1, __VMLINUX_SYMBOL_STR(device_register) },
	{ 0x19998f84, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xf6d20ec1, __VMLINUX_SYMBOL_STR(device_unregister) },
	{ 0xc7eb2e31, __VMLINUX_SYMBOL_STR(dev_set_name) },
	{ 0xdeee9efc, __VMLINUX_SYMBOL_STR(my_bus_type) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=bus";


MODULE_INFO(srcversion, "0F5EB3B8026EA6A9981B706");
