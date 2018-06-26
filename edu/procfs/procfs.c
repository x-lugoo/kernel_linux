#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <generated/utsrelease.h>
#include <linux/version.h>
#include <linux/cred.h>


#define DEVICE_NAME "HELLO_PROCFS"
static struct proc_dir_entry *our_proc_file;

static int proc_show(struct seq_file *m,void *v)
{

	static struct task_struct *curr_task;

	curr_task = get_current();
	seq_printf(m,"current task pid %lu\n",curr_task->pid);
	return 0;
}

static int proc_open(struct inode *inode ,struct file *file)
{
	printk(KERN_INFO "open proc file\n");
	return single_open(file,proc_show,NULL);
}



static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.open  = proc_open,
	.read  = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init myprocfs_init(void)
{

	printk(KERN_INFO "hello procfs :init\n");
	printk(KERN_INFO "HELLO:init module,process \"%s:%i\"\n",current->comm,current->pid);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,10)
	printk(KERN_INFO "old kernel version %s,so use create_proc_entry function\n",UTS_RELEASE);
	our_proc_file = create_proc_entry(DEVICE_NAME,0644,NULL);
	our_proc_file->proc_fops = &fops;
#else
	printk(KERN_INFO " kernel version %s\n",UTS_RELEASE);
	our_proc_file = proc_create(DEVICE_NAME,0644,NULL,&fops);
#endif

	if(!our_proc_file){
		printk(KERN_ALERT "hello procsfs register failure\n");
		return -ENOMEM;
	}
	printk(KERN_INFO "hello procfsf /proc %s has been created\n",DEVICE_NAME);
	return 0;
}
	
static void __exit myprofs_exit(void)
{
	printk(KERN_INFO "cleanup module %s \n",DEVICE_NAME);
	remove_proc_entry(DEVICE_NAME,NULL);

}


module_init(myprocfs_init);
module_exit(myprofs_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("JEFF");

