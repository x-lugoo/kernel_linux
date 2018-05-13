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
	{ 0xa5483600, __VMLINUX_SYMBOL_STR(kmem_cache_destroy) },
	{ 0x115add75, __VMLINUX_SYMBOL_STR(__bread) },
	{ 0xbd02fae2, __VMLINUX_SYMBOL_STR(generic_write_checks) },
	{ 0x34184afe, __VMLINUX_SYMBOL_STR(current_kernel_time) },
	{ 0xb48573b4, __VMLINUX_SYMBOL_STR(dput) },
	{ 0x9f14c292, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xc24baf54, __VMLINUX_SYMBOL_STR(mount_bdev) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x78193dce, __VMLINUX_SYMBOL_STR(mutex_lock_interruptible) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x30fb6d30, __VMLINUX_SYMBOL_STR(d_rehash) },
	{ 0x368eb4a6, __VMLINUX_SYMBOL_STR(kmem_cache_free) },
	{ 0xa6c966e2, __VMLINUX_SYMBOL_STR(sync_dirty_buffer) },
	{ 0x9a52cbc, __VMLINUX_SYMBOL_STR(__brelse) },
	{ 0x6508ec0b, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x85c6e81f, __VMLINUX_SYMBOL_STR(kill_block_super) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x75a28e26, __VMLINUX_SYMBOL_STR(kmem_cache_create) },
	{ 0xea6f01ac, __VMLINUX_SYMBOL_STR(register_filesystem) },
	{ 0xfdd73efb, __VMLINUX_SYMBOL_STR(d_make_root) },
	{ 0xa688bbd1, __VMLINUX_SYMBOL_STR(mark_buffer_dirty) },
	{ 0xd3b111a7, __VMLINUX_SYMBOL_STR(unregister_filesystem) },
	{ 0x9815f5f6, __VMLINUX_SYMBOL_STR(new_inode) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x1e69074a, __VMLINUX_SYMBOL_STR(d_instantiate) },
	{ 0xa4cf68fb, __VMLINUX_SYMBOL_STR(inode_init_owner) },
	{ 0xe914e41e, __VMLINUX_SYMBOL_STR(strcpy) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4811C922EFD39ECBD7AAAF7");
