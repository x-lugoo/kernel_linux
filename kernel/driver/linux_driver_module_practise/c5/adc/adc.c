#include <linux/module.h>
#include <linux/init.h>

#include <linux/clk.h>
#include <asm/io.h>
#include <linux/miscdevice.h>
#include <linux/errno.h>
#include <plat/regs-adc.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/input.h>
#include <linux/serio.h>

#include <linux/sched.h>

static void __iomem* adc_base;
static struct clk* adc_clk;
DECLARE_MUTEX(ADC_LOCK_BT);


#define DEVICE_NAME "mini2440_adc"

static DECLARE_WAIT_QUEUE_HEAD(adc_waitq);
static volatile int ev_adc = 0;
static int adc_data;

EXPORT_SYMBOL(ADC_LOCK_BT);

static irqreturn_t adc_irq(int irq, void* dev_id) {
    if (!ev_adc) {
        adc_data = readl(adc_base + S3C2410_ADCDAT0) & 0x3ff;
        ev_adc = 1;
        wake_up_interruptible(&adc_waitq);
    }

    return IRQ_HANDLED;
}

static int adc_open(struct inode* inode, struct file* filep) {
    int ret;
    ret = request_irq(IRQ_ADC, adc_irq, IRQF_SHARED, DEVICE_NAME, 1);

    if (ret) {
        printk(KERN_ERR"IRQ %d error %d\n", IRQ_ADC, ret);
        return -EINVAL;
    }

    return 0;
}

static void start_adc(void) {
    unsigned int tmp1;


    tmp1 = (1 << 14) | (49 << 6) | (0 << 3); //设置 AD 时钟为 1MHZ
    writel(tmp1, adc_base + S3C2410_ADCCON);

    tmp1 = readl(adc_base + S3C2410_ADCCON);
    tmp1 |= (1 << 0); //将ADCCON寄存器第0位置1，然后通过一下设置ADCCON启动AD转换
    writel(tmp1, adc_base + S3C2410_ADCCON);
}

static ssize_t adc_read(struct file* filp, char* buffer, size_t count, loff_t* ppos) {
    if (down_trylock(&ADC_LOCK_BT)) {
        return -EBUSY;
    }

    if (!ev_adc) {
        if (filp->f_flags & O_NONBLOCK) {
            return -EAGAIN;
        } else {
            start_adc();
            wait_event_interruptible(adc_waitq, ev_adc);
        }
    }

    ev_adc = 0;
    copy_to_user(buffer, (char*)&adc_data, sizeof(adc_data));

    up(&ADC_LOCK_BT);

    return sizeof(adc_data);
}

static int adc_release(struct inode* inode, struct file* filp) {
    return 0;
}

static struct file_operations adc_fops = {
    .owner = THIS_MODULE,
    .open = adc_open,
    .read = adc_read,
    .release = adc_release,
};

static struct miscdevice adc_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &adc_fops,
};


static int __init adc_init(void) {
    int ret;
    adc_clk = clk_get(NULL, "adc");

    if (!adc_clk) {
        printk(KERN_ERR"Failed to find adc clock source\n");
        return -ENOENT;
    }

    clk_enable(adc_clk);

    adc_base = ioremap(S3C2410_PA_ADC, 0x14);

    if (adc_base == NULL) {
        printk(KERN_ERR"Failed to remap adc address!\n");
        ret = -EINVAL;
        goto err_noclk;
    }

    ret = misc_register(&adc_miscdev);

    if (ret) {
        goto err_nomap;
    }

    printk("MINI2440ADC device Initialized!\n");

    return 0;

err_noclk:
    clk_disable(adc_clk);
    clk_put(adc_clk);
err_nomap:
    iounmap(adc_base);
    return ret;
}

static void __exit adc_exit(void) {
    free_irq(IRQ_ADC, 1);
    iounmap(adc_base);

    if (adc_clk) {
        clk_disable(adc_clk);
        clk_put(adc_clk);
        adc_clk = NULL;
    }

    misc_deregister(&adc_miscdev);
    printk("MINI2440ADC device Removed!\n");
}

module_init(adc_init);
module_exit(adc_exit);


MODULE_LICENSE("GPL");

