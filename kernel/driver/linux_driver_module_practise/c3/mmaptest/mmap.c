#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/mm.h>
#include<linux/device.h>
#include <linux/miscdevice.h>


#define DEV_NAME "mmapled"

int mmapled_open(struct inode* inode, struct file* filp) {

    return 0;
}
int mmapled_close(struct inode* inode , struct file* filp) {
    return 0;
}

/*mmap系统调用函数*/
int mmapled_mmap(struct file* filp, struct vm_area_struct* vma) {
    int ret;
    vma->vm_flags |= VM_RESERVED;
    vma->vm_flags |= VM_IO;
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
    /*vma->vm_pgoff为用户层off>>PAGE_SHIFT,即物理地址的页帧号,映射大小必为PAGE_SIZE整数倍*/
    ret = remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot);

    if (ret) {
        printk("remap_pfn_range err!\n");
        return -EAGAIN;
    }

    printk("In %s,pgoff = %lx,start = %lx,end = %lx\n", __func__, vma->vm_pgoff, vma->vm_start, vma->vm_end);
    return 0;
}

/*文件操作结构体*/
struct file_operations mmapled_fops = {
    .owner = THIS_MODULE,
    .open = mmapled_open,
    .release = mmapled_close,
    .mmap = mmapled_mmap,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEV_NAME,
    .fops = &mmapled_fops,
};



/*驱动程序入口函数*/
int mmapled_init(void) {
    int ret;
    misc_register(&misc);
    return 0;
}
/*驱动程序出口函数*/
void mmapled_exit(void) {
    misc_deregister(&misc);
}
module_init(mmapled_init);
module_exit(mmapled_exit);
MODULE_LICENSE("GPL");