#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/i2c.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/delay.h>


#define AT24C08B_MAJOR 250


static const struct i2c_device_id at24c08b_id[] = {
    { "at24c08b_driver", 0 }
};
MODULE_DEVICE_TABLE(i2c, at24c08b_id);

struct at24c08b_dev {
    struct i2c_client client;
    unsigned int addr;
    char name[30];
    unsigned short current_pointer;
    struct cdev cdev;
};
struct at24c08b_dev* at24c08b_devp;


static unsigned short normal_i2c[] = {
    0x50, I2C_CLIENT_END
};
static unsigned short ignore[2] = {
    I2C_CLIENT_END, I2C_CLIENT_END
};
static unsigned short probe[2] = {
    I2C_CLIENT_END, I2C_CLIENT_END
};

static struct i2c_client_address_data addr_data = {
    .normal_i2c = normal_i2c,
    .probe = probe,
    .ignore = ignore,
    .forces = NULL,
};


static int at24c08b_attach(struct i2c_adapter* adapter, int address, int kind) {
    int ret;

    at24c08b_devp->client.driver = &at24c08b_driver,
                          at24c08b_devp->client.addr = address,
                                                at24c08b_devp->client.adapter = adapter,
                                                                      at24c08b_devp->client.flags = 0,
                                                                                            strlcpy(at24c08b_devp->client.name, "at24c08b eeprom", I2C_NAME_SIZE);

    ret = i2c_attach_client(&at24c08b_devp->client);

    if (ret) {
        printk(KERN_NOTICE"i2c_attach_client error");
    }

    return 0;
}

static int at24c08b_probe(struct i2c_adapter* adapter) {
    return i2c_probe(adapter, &addr_data, at24c08b_attach);
}

static int at24c08b_detach_client(struct i2c_client* client) {
    return i2c_detach_client(client);
}

static struct i2c_driver at24c08b_driver = {
    .driver =
    {
        .name = "at24c08b_driver",
        .owner = THIS_MODULE,
    },
    .attach_adapter = at24c08b_probe,
    .detach_client = at24c08b_detach_client,
    .id_table = at24c08b_id,
};




static int at24c08b_open(struct inode* inode, struct file* file) {
    file->private_data = at24c08b_devp;
    return 0;
}
static ssize_t at24c08b_read(struct file* file, char* buf, size_t count, loff_t* ppos) {
    int i = 0;
    int transferred = 0;
    int ret, my_buf[512];
    struct at24c08b_dev* dev = (struct at24c08b_dev*)file->private_data;

    dev->current_pointer = *ppos;

    if (i2c_check_functionality(dev->client.adapter, I2C_FUNC_SMBUS_READ_WORD_DATA)) {
        while (transferred < count) {
            ret = i2c_smbus_read_word_data(&dev->client, dev->current_pointer + i);
            my_buf[i++] = (unsigned short)(ret & 0xFF);
            my_buf[i++] = (unsigned short)(ret >> 8);
            transferred += 2;
        }

        copy_to_user(buf, (void*)my_buf, transferred);
        dev->current_pointer += transferred;
    }

    return transferred;
}
static ssize_t at24c08b_write(struct file* file, char* buf, size_t count, loff_t* ppos) {
    int i = 0;
    int transferred = 0;
    int ret, my_buf[512];
    struct at24c08b_dev* dev = (struct at24c08b_dev*)file->private_data;

    dev->current_pointer = *ppos;

    if (i2c_check_functionality(dev->client.adapter, I2C_FUNC_SMBUS_WRITE_WORD_DATA)) {
        copy_from_user(my_buf, buf, count);

        while (transferred < count) {
            ret = i2c_smbus_write_word_data(&dev->client, dev->current_pointer + i, my_buf[i]);
            i += 2;
            transferred += 2;
        }

        dev->current_pointer += transferred;
    }

    return transferred;
}
static int at24c08b_ioctl(struct inode* inodep, struct file* file, unsigned int cmd, unsigned long arg) {
    return 0;
}
static int at24c08b_release(struct inode* inodep, struct file* file) {
    file->private_data = NULL;
    return 0;
}
static const struct file_operations at24c08b_fops = {
    .owner = THIS_MODULE,
    .open = at24c08b_open,
    .read = at24c08b_read,
    .write = at24c08b_write,
    .ioctl = at24c08b_ioctl,
    .release = at24c08b_release,
};
static void at24c08b_setup_cdev(struct at24c08b_dev* dev, int index) {
    int err, devnum = MKDEV(at24c08b_major, index);
    cdev_init(&dev->cdev, &at24c08b_fops);
    dev->cdev.owner = THIS_MODULE;
    err = cdev_add(&dev->cdev, devnum, 1);

    if (err) {
        printk(KERN_NOTICE"Error %d adding at24c08b %d", err, index);
    }
}
static int __init at24c08b_init(void) {
    int ret;
    dev_t devnum = MKDEV(at24c08b_major, 0);

    if (at24c08b_major) {
        ret = register_chrdev_region(devnum, 1, "at24c08b");
    } else {
        ret = alloc_chrdev_region(&devnum, 0, 1, "at24c08b");
        at24c08b_major = MAJOR(devnum);
    }

    if (ret < 0) {
        return ret;
    }

    at24c08b_devp = kmalloc(sizeof(struct at24c08b_dev), GFP_KERNEL);

    if (!at24c08b_devp) {
        ret = -ENOMEM;
        goto fail_malloc;
    }

    memset(at24c08b_devp, 0, sizeof(struct at24c08b_dev));
    at24c08b_setup_cdev(at24c08b_devp, 0);
    i2c_add_driver(&at24c08b_driver);
    return 0;
fail_malloc:
    unregister_chrdev_region(devnum, 1);
    return ret;
}
void at24c08b_exit(void) {
    cdev_del(&at24c08b_devp->cdev);
    kfree(at24c08b_devp);
    unregister_chrdev_region(MKDEV(at24c08b_major, 0), 1);
    i2c_del_driver(&at24c08b_driver);
}
MODULE_DESCRIPTION("at24c08b eeprom driver");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("lxlong");
module_init(at24c08b_init);
module_exit(at24c08b_exit);
