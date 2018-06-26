#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>

static DEFINE_SPINLOCK(static_lock);

spinlock_t dynamic_lock;

static int __init myspinlock_init(void)
{
	unsigned long flags;

	/*static spinlock irq safe*/
	printk(KERN_INFO "myspinlock:%s\n",__FUNCTION__);
	spin_lock_irqsave(&static_lock,flags);
	printk(KERN_INFO "myspinlock : I have acquired a staic spinlock\n");
	spin_unlock_irqrestore(&static_lock,flags);

	/*static spinlock cannot use within interrupt handlers**/
	spin_lock(&static_lock);
	spin_unlock(&static_lock);


	/*dynamic spinlock irq safe*/
	spin_lock_init(&dynamic_lock);
	spin_lock_irqsave(&dynamic_lock,flags);
	spin_unlock_irqrestore(&dynamic_lock,flags);
	return 0;
}

static void __exit myspinlock_exit(void)
{
	printk(KERN_INFO "myspinlock %s\n",__FUNCTION__);
}
module_init(myspinlock_init);
module_exit(myspinlock_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("JEFF");


