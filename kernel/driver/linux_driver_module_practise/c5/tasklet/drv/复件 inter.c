#include<linux/module.h>
#include<linux/init.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/kernel.h>      //可以使用ptintk函数  
#include<linux/fs.h>
#include<linux/interrupt.h>   //与中断相关的头文件  
#include<linux/cdev.h>           //字符设备的注册  
#include<linux/err.h>           //出错检查函数  
#include<linux/slab.h>         //与内存分配相关  
#include<linux/sched.h>        //与中断相关的头文件  
#include<asm/io.h>             //操作IO端口  
#include<linux/wait.h>         //等待队列   
#include<asm/uaccess.h>         //用户和内核空间数据的拷贝  
#include<linux/delay.h>          //时间延迟函数
#include <linux/miscdevice.h>
#include<mach/irqs.h>
#include <linux/irq.h>

#include <asm/irq.h>

#define DEVICE_NAME "/dev/2440int"          //键盘设备名   

#define GPGCON_ADDR  0x56000060   /*key*/
#define GPGDAT_ADDR  0x56000064

u32*             IMM32GPGCON;
u32*             IMM32GPGDAT;

static int eint2_value = 0;              //中断2的值
static int eint_number = 0;              //产生中断的次数


static int TimerCount = 0;                   //超时标志

#define TIMER_DELAY  (HZ/50)                 /* 消抖动 延时:1秒*/
#define TIMEOUT 3                            //连读三次去抖动

static struct timer_list ring_timer;         //定义一个时钟

static  wait_queue_head_t wq ;               //声明等待队列

//spinlock_t repeat_lock;
spinlock_t lock;                             //声明自旋锁


#define BLOCK   1                            //阻塞方式
#define NONBLOCK    0                        //非阻塞方式

static u_int FileMode = NONBLOCK;            //初始化为非阻塞



static int requestIrq();                     //声明中断申请函数


int get_eint2_value() {
    unsigned long IOValue;

    IOValue = *IMM32GPGDAT ;

    if (IOValue & 0x00000004) {                      //有eint2
        return 0;
    } else {
        return 1;
    }
}


static inline void  timer_handler(unsigned long data) { //定时中断服务函数，定时读多次，防止抖动
    if (!get_eint2_value()) {                      /* 如果没有产生外部中断2，说明是抖动，屏蔽了*/
        TimerCount = 0;                          //  清0
        del_timer(&ring_timer);                  //删除定时器
        local_irq_enable();                             //使能中断
    }

    TimerCount++;                                  //有中断2产生

    if (TimerCount == TIMEOUT) {                   //看看是不是第三次定时都有中断2，确认是有按键按下
        eint2_value = 1;                        //确认一次中断，将其值置为1；
        eint_number = eint_number + 1;          //中断次数加一

        printk("int coming %d\n", eint_number); //打印外部中断次数

        if (FileMode == BLOCK) {                //不执行
            wake_up_interruptible(&wq);    //阻塞，但可被中断
        }


        del_timer(&ring_timer);                 //删除定时器
        TimerCount = 0;
        local_irq_enable();
    } else {
        ring_timer.expires = jiffies + TIMER_DELAY;  //重新赋值定时器的数值
        add_timer(&ring_timer);                      //重新启动定时器
    }
}





//使能中断
static  void enableIrq() {
    //清除SRCPND寄存器中eint2相应位
    *IMM32GPGDAT = 0x00000004;

    //使能中断
    enable_irq(IRQ_EINT2);

}

static ssize_t
S3C2440_irint_read(struct file* filp, char* buffer, size_t count, loff_t* ppos) {

    //  printk("<0>,eint_number %d\n",eint_number);
    if (FileMode == BLOCK) {                    //不执行
        if (eint2_value == 0) {                 //如果没有数据
            interruptible_sleep_on(&wq);        //一直睡眠等待

            //          if(signal_pending(current))
            //              return -ERESTARTSYS;
        }
    }

    count = sizeof(eint_number);
    copy_to_user(buffer, &eint_number, count);  //将按键按下的值eint_number拷贝到用户空间
    eint2_value = 0;                            //数据读完后，再次清除
    return count;

}

static int S3C2440_irint_open(struct inode* inode, struct file* filp) {
    return 0;
}

static int s3c2440_irint_release(struct inode* inode, struct file* filp) {
    disable_irq(IRQ_EINT2);                  //关闭中断2

    printk("release device\n");

    return 0;
}

static struct file_operations ring_fops = {
owner:
    THIS_MODULE,
open:
    S3C2440_irint_open,
read:
    S3C2440_irint_read,
release:
    s3c2440_irint_release,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &ring_fops ,
};


static void irint_interrupt(int irq, void* dev_id, struct pt_regs* regs)    //外部中断2的服务函数

{

    spin_lock_irq(&lock);                                                   //锁起来
    disable_irq(IRQ_EINT2);                                                 //屏弊中断2

    //启动定时器
    TimerCount = 0;
    ring_timer.expires = jiffies + TIMER_DELAY;                          //设置定时时间
    add_timer(&ring_timer);                                              //启动定时器（定时时间要远远大于抖动时间，小于第二次按中断2的时间）

    //  enableIrq();
    spin_unlock_irq(&lock);                                                //解锁
}

static int requestIrq() {
    int ret;

    ret = request_irq(IRQ_EINT8, irint_interrupt, IRQ_TYPE_EDGE_BOTH,
                      DEVICE_NAME, NULL);                                         //注册中断服务函数

    if (ret) {
        goto eint2_failed;
    }

    printk("requestIrq\n");
    return 0;


eint2_failed:
    free_irq(IRQ_EINT8, NULL);

eint_failed:

    printk(DEVICE_NAME ": IRQ Requeset Error\n");

    return ret;

}

static int __init s3c2440_irint_init(void) {
    int ret;

    misc_register(&misc);

    if (requestIrq() != 0) {
        misc_deregister(&misc);
        return -1;
    }

    IMM32GPGCON = (u32*) ioremap(GPGCON_ADDR, 4); /* key */
    IMM32GPGDAT = (u32*) ioremap(GPGDAT_ADDR, 4);


    init_timer(&ring_timer);                                                   //初始化定时器
    ring_timer.function = timer_handler;                                       //设置定时中断服务函数
    lock = SPIN_LOCK_UNLOCKED;                                                 //初始化自旋锁
    init_waitqueue_head(&wq);                                                  //初始化等待队列
    enableIrq();                                                               //使能中断



    return 0;
}

static void __exit s3c2440_irint_exit(void) {

    /*注销设备*/
    misc_deregister(&misc);

    /*释放中断*/
    free_irq(IRQ_EINT8, NULL);
}

module_init(s3c2440_irint_init);
module_exit(s3c2440_irint_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lxlong");
