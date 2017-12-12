//-------------------------------------------------------------------
//	cr4.c
//
//	This module creates a pseudo-file (named '/proc/cr4') which
//	allows applications to obtain the current value held in the
//	processor's CR4 register.  Individual bits in this register
//	are set by the operating system to either enable or disable
//	special features of the Pentium's operations (including the
//	scheme by which the CPU translates virtual memory-addresses 
//	to physical memory-addresses).  Refer to Volume 3 of "IA-32
//	Intel Architecture Software Developer's Manual" (Chapter 2)
//	for detailed explanation of all bits in Control Register 4.
//
//	NOTE: Developed and tested with Linux kernel version 2.6.9.
//
//	programmer: ALLAN CRUSE
//	written on: 07 JAN 2005
//	revised by jeff:28 NOV 2017
//-------------------------------------------------------------------

#include <linux/module.h>	// (for module_init()) 
#include <linux/proc_fs.h>	// (for proc_create()) 
#include <linux/sched.h>	// (for init_task())

static char modname[] = "task_list";

static ssize_t my_get_info(struct file *filp,char __user *buf,
	       size_t count,loff_t *ppos)	
{
	struct task_struct	*task = &init_task;
	int    n_elt = 0, len = 0;

	do{
		++n_elt;
		len += sprintf( buf+len, "%5d ", task->pid );
		len += sprintf( buf+len, "%lu ", task->state );
		len += sprintf( buf+len, "%-15s ", task->comm );
		if ( ( n_elt % 3 ) == 0 ) len += sprintf( buf+len, "\n" );
		task = next_task( task );
		}
	while ( task != &init_task );

	len += sprintf( buf+len, "\n" );
	return	len;
}

static const struct file_operations test_proc = 
{
	.owner = THIS_MODULE,
	.read  = my_get_info,
};
int task_list_module( void )
{
	printk( "<1>\nInstalling \'%s\' module\n", modname );
	proc_create( modname, 0, NULL, &test_proc );
	return	0;  // SUCCESS
}


void clean_task_list_module( void )
{
	remove_proc_entry( modname, NULL );
	printk( "<1>Removing \'%s\' module\n", modname );
}

module_init(task_list_module);
module_exit(clean_task_list_module);
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("JEFF");
