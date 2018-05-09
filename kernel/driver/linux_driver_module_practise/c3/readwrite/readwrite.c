#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/pci.h>
#include<linux/moduleparam.h>
#include<linux/init.h>
#include<linux/string.h>
#include<linux/device.h>
#include<asm/uaccess.h>
#include<asm/unistd.h>
#include<asm/uaccess.h>
#include <linux/miscdevice.h>  //attention


#define SUCCESS 0
#define DEVICE_NAME "readwrite"
#define BUF_LEN 80


static char msg[BUF_LEN];


static int device_open(struct inode* inode, struct file* file) {
    memset(msg, 0, sizeof(msg));

    return SUCCESS;
}

static int device_release(struct inode* inode, struct file* file) {
    return 0;
}

static ssize_t device_read(struct file* filp, char* buffer, size_t length, loff_t* offset) {

    unsigned char  off;
    off = (*offset);

    printk("read:length=%d,offset:%d\n", length, off);

    printk("read:%d,%d,%d\n", msg[0], msg[1], msg[2]);
    copy_to_user(buffer, msg + off, length);

    printk("read:%d,%d,%d\n", buffer[0], buffer[1], buffer[2]);
    return length;
}


static ssize_t device_write(struct file* filp, const char* buff, size_t length, loff_t* offset) {
    unsigned char  off;

    off = (*offset);

    printk("write:length=%d,offset:%d\n", length, off);

    copy_from_user(msg + off, buff, length);

    printk("write:%d,%d,%d\n", msg[0], msg[1], msg[2]);

    return length;
}

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &fops,
};


int misc_init(void) {
    misc_register(&misc);
    printk(DEVICE_NAME"\tinitialized\n");
    return SUCCESS;
}

void misc_exit(void) {
    misc_deregister(&misc);
    printk("cleanup done\n");
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_AUTHOR("lxlong");
MODULE_LICENSE("GPL");

