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

#include <linux/module.h>	// for init_module() 
#include <linux/proc_fs.h>	// for create_proc_info_entry() 
#include <linux/sched.h>
#include <linux/mm.h>

static char modname[] = "vma";

static ssize_t my_get_info(struct file *filp,char __user *buf,
	       size_t count,loff_t *ppos)	
{
	struct task_struct	*tsk = current;
	struct vm_area_struct	*vma;
	unsigned long 		ptdb;
	int			i = 0, len = 0;
	
	// display title
	len += sprintf( buf+len, "\n\nList of the Virtual Memory Areas " );
	len += sprintf( buf+len, "for task \'%s\' ", tsk->comm );
	len += sprintf( buf+len, "(pid=%d)\n", tsk->pid );

	// loop to traverse the list of the task's vm_area_structs
	vma = tsk->mm->mmap;
	while ( vma )
		{
		char	ch;	
		len += sprintf( buf+len, "\n%3d ", ++i );
		len += sprintf( buf+len, " vm_start=%08lX ", vma->vm_start );
		len += sprintf( buf+len, " vm_end=%08lX  ", vma->vm_end );

		ch = ( vma->vm_flags & VM_READ ) ? 'r' : '-';
		len += sprintf( buf+len, "%c", ch );
		ch = ( vma->vm_flags & VM_WRITE ) ? 'w' : '-';
		len += sprintf( buf+len, "%c", ch );
		ch = ( vma->vm_flags & VM_EXEC ) ? 'x' : '-';
		len += sprintf( buf+len, "%c", ch );
		ch = ( vma->vm_flags & VM_SHARED ) ? 's' : 'p';
		len += sprintf( buf+len, "%c", ch );
		
		vma = vma->vm_next;
		}
	len += sprintf( buf+len, "\n" );

	// display additional information about tsk->mm
	asm(" movl %%cr3, %%ecx \n movl %%ecx, %0 " : "=m" (ptdb) );
	len += sprintf( buf+len, "\nCR3=%08lX ", ptdb );
	len += sprintf( buf+len, " mm->pgd=%p ", tsk->mm->pgd );
	len += sprintf( buf+len, " mm->map_count=%d ", tsk->mm->map_count );
	len += sprintf( buf+len, "\n\n" );

	return	len;
}

static const struct file_operations test_proc = 
{
	.owner = THIS_MODULE,
	.read  = my_get_info,
};
int vma_module( void )
{
	printk( "<1>\nInstalling \'%s\' module\n", modname );
	proc_create( modname, 0, NULL, &test_proc );
	return	0;  // SUCCESS
}


void clean_vma_module( void )
{
	remove_proc_entry( modname, NULL );
	printk( "<1>Removing \'%s\' module\n", modname );
}

module_init(vma_module);
module_exit(clean_vma_module);
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("JEFF");
