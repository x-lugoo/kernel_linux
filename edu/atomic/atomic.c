#include <linux/init.h>
#include <linux/module.h>
#include <linux/atomic.h>
#include <linux/interrupt.h>

#define DEBUG(m) printk("myatomic : " m "current value =%d\n",atomic_read(&atom));



static int __init myatomic_init(void)
{
	atomic_t atom;
	printk(KERN_INFO "myatomic:%s\n",__FUNCTION__);
	atomic_set(&atom,0);
	DEBUG("atom set");
	atomic_dec(&atom);
	DEBUG("atom dec");
	atomic_inc(&atom);
	DEBUG("atom inc");
	atomic_add(100,&atom);
	DEBUG("atom add");
	atomic_sub(50,&atom);
	DEBUG("atom sub");



	
	if(atomic_sub_and_test(50,&atom)) /*compare with 0*/
		DEBUG("atomic_sub_and_test");
	if(atomic_inc_and_test(&atom))
		DEBUG("atomic_inc_and_test");
	if(atomic_dec_and_test(&atom))
		DEBUG("atomic_dec_and_test");

	return 0;
}

static void __exit myatomic_exit(void)
{
	printk(KERN_INFO "myatomic: %s\n",__FUNCTION__);
}

module_init(myatomic_init);
module_exit(myatomic_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("JEFF");
