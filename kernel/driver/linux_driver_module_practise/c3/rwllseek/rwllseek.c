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
#include <linux/errno.h>
#include <linux/miscdevice.h>  //attention


#define SUCCESS 0
#define DEVICE_NAME "rwllseek"
#define BUF_LEN 80


#define LEN_BUF 256
static unsigned char Buffer[LEN_BUF];


static int device_open(struct inode* inode, struct file* file) {
    memset(Buffer, 0, sizeof(Buffer));

    return SUCCESS;
}

static int device_release(struct inode* inode, struct file* file) {
    return 0;
}

static ssize_t device_read(struct file* filp, char* buffer, size_t count, loff_t* f_pos) {
    if (*f_pos >= LEN_BUF) {
        return 0;
    }

    if (count > (LEN_BUF - *f_pos)) {
        count = 256 - *f_pos;
    }

    if (copy_to_user(buffer, Buffer + *f_pos, count)) {
        return -EFAULT;
    }

    *f_pos += count;

    return count;
}


static ssize_t device_write(struct file* filp, const char* buff, size_t count, loff_t* f_pos) {
    ssize_t err = -ENOMEM;

    if (*f_pos >= LEN_BUF) {
        goto ERR;
    }

    if (count > (LEN_BUF - *f_pos)) {
        count = 256 - *f_pos;
    }

    if (copy_from_user(Buffer + *f_pos, buff, count)) {
        err = -EFAULT;
        goto ERR;
    }

    *f_pos += count;

    return count;

ERR:
    return err;

}


static ssize_t test_driver_llseek(struct file* filp, size_t count, loff_t off, int whence) {
    loff_t pos;
    pos = filp->f_pos;

    switch (whence) {
    case 0: {
            pos = off;
            break;
        }

    case 1: {
            pos += off;
            break;
        }

    case 2:
    default: {
            return -EINVAL;
        }
    }

    if (pos > LEN_BUF || pos < 0) {
        return -EINVAL;
    }

    filp->f_pos = pos;
    return filp->f_pos;
}



static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
    .llseek = test_driver_llseek,
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

