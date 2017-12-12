//-------------------------------------------------------------------
//	mm.c
//
//	This module creates a pseudo-file (named '/proc/mm') which
//	displays some info from a task's memory-manager structure.
//
//	NOTE: Written and tested with Linux kernel version 2.4.26.
//
//	programmer: ALLAN CRUSE
//	written on: 21 SEP 2004
//-------------------------------------------------------------------

#include <linux/module.h>	// for init_module() 
#include <linux/proc_fs.h>	// for create_proc_read_entry() 
#include <linux/sched.h>

static char modname[] = "mm";
static ssize_t my_get_info(struct file *filp,char __user *buf,
	       size_t count,loff_t *ppos)	
{
	struct task_struct	*tsk = current;
	struct mm_struct	*mm = tsk->mm;
	int			len;

	len = sprintf( buf, "\nInfo from the Memory Manager structure " );
	len += sprintf( buf+len, "for task \'%s\' ", tsk->comm );
	len += sprintf( buf+len, "(pid=%d) \n", tsk->pid );
	len += sprintf( buf+len, "   pgd=%08lX  ", mm->pgd );
	len += sprintf( buf+len, "mmap=%08lX  ", mm->mmap );
	len += sprintf( buf+len, "map_count=%d  ", mm->map_count );
	len += sprintf( buf+len, "mm_users=%d  ", mm->mm_users );
	len += sprintf( buf+len, "mm_count=%d  ", mm->mm_count );
	len += sprintf( buf+len, "\n" );
	len += sprintf( buf+len, "    start_code=%08lX  ", mm->start_code );
	len += sprintf( buf+len, " end_code=%08lX\n", mm->end_code );
	len += sprintf( buf+len, "    start_data=%08lX  ", mm->start_data );
	len += sprintf( buf+len, " end_data=%08lX\n", mm->end_data );
	len += sprintf( buf+len, "     start_brk=%08lX  ", mm->start_brk );
	len += sprintf( buf+len, "      brk=%08lX\n", mm->brk );
	len += sprintf( buf+len, "     arg_start=%08lX  ", mm->arg_start );
	len += sprintf( buf+len, "  arg_end=%08lX\n", mm->arg_end );
	len += sprintf( buf+len, "     env_start=%08lX  ", mm->env_start );
	len += sprintf( buf+len, "  env_end=%08lX\n", mm->env_end );
	len += sprintf( buf+len, "   start_stack=%08lX  ", mm->start_stack );
	len += sprintf( buf+len, "end_stack=%08lX\n", 0xC0000000 );
	len += sprintf( buf+len, "\n" );
	return	len;
}

static const struct file_operations test_proc = 
{
	.owner = THIS_MODULE,
	.read  = my_get_info,
};
int mm_module( void )
{
	printk( "<1>\nInstalling \'%s\' module\n", modname );
	proc_create( modname, 0, NULL, &test_proc );
	return	0;  // SUCCESS
}


void clean_mm_module( void )
{
	remove_proc_entry( modname, NULL );
	printk( "<1>Removing \'%s\' module\n", modname );
}

module_init(mm_module);
module_exit(clean_mm_module);
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("JEFF");

