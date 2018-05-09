#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<asm/uaccess.h>

#define SUCCESS 0

#define DEVICE_NAME "chardev"

#define BUF_LEN 80

static int major;
static int Device_open = 0;
static char msg[BUF_LEN];
static char *msg_ptr;


static int device_open(struct inode* inode, struct file* file)
{        
	static int counter = 0;        
	if (Device_open)        
	{                
		return -EBUSY;        
	}        
	Device_open++;        
	sprintf(msg, "I already told you %d times hello world\nDevice_open=%d\n" , counter++, Device_open);
	msg_ptr = msg;        
	try_module_get(THIS_MODULE);        
	return SUCCESS;
}
static int device_release(struct inode* inode, struct file* file)
{        
	Device_open--;        
	module_put(THIS_MODULE);        
	return 0;
}
static ssize_t device_read(struct file* filp, char *buffer, size_t length, loff_t *offset)
{        
	int bytes_read = 0;        
	if (*msg_ptr == '\0')               
	 return 0;        
	 printk(KERN_ALERT "length=%d\n", length);        
	 while (length && *msg_ptr)        
	 {                
	 	put_user(*(msg_ptr++), buffer++);                
	 	length--;                
	 	bytes_read++;        
	 }        
	 return bytes_read;
}
static ssize_t device_write(struct file* filp, const char *buff, size_t len, loff_t *off)
{        
	printk(KERN_ALERT "Sorry, this operation isn't supported\n");        
	return -EINVAL;
}

static struct file_operations fops =
{        .read = device_read,
         .write = device_write,
         .open = device_open,
         .release = device_release,
};

int init_module(void)
{
        major = register_chrdev(0, DEVICE_NAME, &fops);
        if (major < 0)        
        {                
        	printk(KERN_ALERT "Registering char device failed with %d\n", major);                
        	return major;        
        }        
        printk(KERN_INFO "I was assigned major number %d.\n", major);
        return SUCCESS;
}

void cleanup_module(void)
{        int ret = unregister_chrdev(major, DEVICE_NAME);        
         if (ret < 0)        
         {                
         	printk(KERN_ALERT "Error in unregister chrdev %d\n", major);        
         }
}

module_init(init_module);
module_exit(cleanup_module);

MODULE_AUTHOR("lxlong");
MODULE_LICENSE("GPL");

