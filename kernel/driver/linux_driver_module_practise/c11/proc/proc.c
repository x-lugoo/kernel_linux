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
#include <linux/proc_fs.h>


#define DEBUG_OFF  0
#define DEBUG_ON   1

int debugFlag = DEBUG_OFF;

#define     debug_msg(arg...)  \
    do{                \
        if(DEBUG_ON == debugFlag) \
        {                         \
            printk(arg);\
        }              \
    }while(0)



int debug_proc_entry(char* buf, char** start, off_t offset,
                     int len, int* eof, void* data) {
    char* p;
    int size = 0;
    p = buf;

    if (offset != 0) {
        goto end;
    }

    switch ((long)data) {
    case DEBUG_ON:
        p += sprintf(p, "\ndebug on\n");
        debugFlag = DEBUG_ON;
        break;

    case DEBUG_OFF:
        p += sprintf(p, "\ndebug off\n");
        debugFlag = DEBUG_OFF;
        break;
    }

end:
    size = p - buf;

    if (size <= offset + len) {
        *eof = 1;
    }

    *start = buf + offset;
    size -= offset;

    if (size > len) {
        size = len;
    }

    if (size < 0) {
        size = 0;
    }

    return size;
}


void debug_proc_init(char* name) {
    char file[20];
    sprintf(file, "%s_on", name);
    create_proc_read_entry(file, 0, NULL, debug_proc_entry,
                           (void*)DEBUG_ON);
    sprintf(file, "%s_off", name);
    create_proc_read_entry(file, 0, NULL, debug_proc_entry,
                           (void*)DEBUG_OFF);
}

void debug_proc_remove(char* name) {
    char file[20];
    sprintf(file, "%s_on", name);
    remove_proc_entry(file, NULL);
    sprintf(file, "%s_off", name);
    remove_proc_entry(file, NULL);
}





typedef struct iotest {
    int a;
    int b;
    char c[4];
} iotest;


#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUF_LEN 80


#define GPBCON_ADDR  0x56000010

#define GPBDAT_ADDR  0x56000014

u32*     IMM32GPBCON;
u32*           IMM32GPBDAT;




static int Device_open = 0;
static char msg[BUF_LEN];
static char* msg_ptr;


static int device_open(struct inode* inode, struct file* file) {
    static int counter = 0;

    if (Device_open) {
        return -EBUSY;
    }

    Device_open++;

    sprintf(msg, "I already told you %d times hello world\nDevice_open=%d\n",
            counter++, Device_open);
    msg_ptr = msg;
    try_module_get(THIS_MODULE);
    return SUCCESS;
}

static int device_release(struct inode* inode, struct file* file) {
    Device_open--;
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file* filp, char* buffer, size_t length, loff_t* offset) {
    int bytes_read = 0;

    if (*msg_ptr == '\0') {
        return 0;
    }

    printk(KERN_ALERT "length=%d\n", length);

    while (length && *msg_ptr) {
        put_user(*(msg_ptr++), buffer++);
        length--;
        bytes_read++;
    }

    return bytes_read;
}


static ssize_t device_write(struct file* filp, const char* buff, size_t len, loff_t* off) {
    printk(KERN_ALERT "Sorry, this operation isn't supported\n");
    return -EINVAL;
}


/*--------------------------------------------------------------------------
 * Ioctl
 *------------------------------------------------------------------------*/
int device_ioctl(struct inode* inodp, struct file* filp, unsigned int cmd, unsigned long arg) {
    iotest* usrData;

    switch (cmd) {
    case 0:
        *IMM32GPBDAT  = 0x0;
        break;

    case 1:
        *IMM32GPBDAT  = 0x1E0;
        break;

    case 0x22:
        usrData = (iotest*)arg;
        debug_msg("frome user a:%d,b:%d\n", usrData->a, usrData->b);
        debug_msg("now chang user data c to 1 2 3 4\n");
        usrData->c[0] = 1;
        usrData->c[1] = 2;
        usrData->c[2] = 3;
        usrData->c[3] = 4;
        break;

    default:
        break;
    }

    return 1;
}


static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
    .ioctl = device_ioctl,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &fops,
};


int misc_init(void) {
    IMM32GPBCON = (u32*) ioremap(GPBCON_ADDR, 4);
    IMM32GPBDAT = (u32*) ioremap(GPBDAT_ADDR, 4);
    *IMM32GPBCON = 0x15400;
    *IMM32GPBDAT  = 0x0;
    misc_register(&misc);

    debug_proc_init("miscproc");
    printk(DEVICE_NAME"/tinitialized/n");
    return SUCCESS;
}

void misc_exit(void) {
    misc_deregister(&misc);
    printk("cleanup done\n");
    debug_proc_remove("miscproc");
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_AUTHOR("lxlong");
MODULE_LICENSE("GPL");

