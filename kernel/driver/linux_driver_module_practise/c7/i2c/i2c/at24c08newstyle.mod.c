#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
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
    { 0xcb86d6b1, "module_layout" },
    { 0x98082893, "__copy_to_user" },
    { 0x6e0000c8, "i2c_smbus_read_byte_data" },
    { 0x6974472b, "i2c_del_driver" },
    { 0x48bfaef9, "i2c_register_driver" },
    { 0xb85970de, "i2c_smbus_write_byte_data" },
    { 0x17a142df, "__copy_from_user" },
    { 0x105e2727, "__tracepoint_kmalloc" },
    { 0x682d4d0c, "kmalloc_caches" },
    { 0x29537c9e, "alloc_chrdev_region" },
    { 0x1e699a1, "cdev_add" },
    { 0xe8c9be6a, "cdev_init" },
    { 0xfa2a45e, "__memzero" },
    { 0x706e5e4f, "kmem_cache_alloc" },
    { 0xd8e484f0, "register_chrdev_region" },
    { 0xea147363, "printk" },
    { 0x7485e15e, "unregister_chrdev_region" },
    { 0x37a0cba, "kfree" },
    { 0xe53a2294, "cdev_del" },
    { 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
    "depends=";

MODULE_ALIAS("i2c:at24c08b");

MODULE_INFO(srcversion, "71B47545B8B9118EFF7C007");
