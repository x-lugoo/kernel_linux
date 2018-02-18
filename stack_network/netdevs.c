//-------------------------------------------------------------------
//	netdevs.c
//
//	This module create a pseudo-file named '/proc/netdevs' that 
//	displays a list of the current 'struct net_device' objects.
//
//	NOTE: Written and tested for Linux kernel version 2.6.22.5.
//
//	programmer: ALLAN CRUSE
//	written on: 16 JAN 2008
//	revised on: 25 JAN 2008 -- to also show size of 'net_device'  
//	modified by jeff_xie 02/09/2018
//-------------------------------------------------------------------
#include <linux/module.h>	// for init_module() 
#include <linux/proc_fs.h>	// for create_proc_info_entry() 
#include <linux/netdevice.h>	// for 'struct net_device'
#include <asm/io.h>		// for virt_to_phys()

char modname[] = "netdevs";
char legend[] = "List of the kernel's current 'struct net_device' objects";
char header[] = "MemAddress   HardwareAddress  ifindex  Name";

static ssize_t my_get_info(struct file *filp,char __user *buf,
               size_t count,loff_t *ppos)   
{
	struct net_device	*dev;
	int			i, len = 0;
	
	len += sprintf( buf+len, "\n %s\n\n %s\n", legend, header );
	for_each_netdev( &init_net,dev ) 
		{
		unsigned long	phys_address = virt_to_phys( dev );
		unsigned char	*mac = dev->dev_addr;
		len += sprintf( buf+len, " 0x%08lX  ", phys_address );
		for (i = 0; i < 6; i++) 
		    len += sprintf( buf+len, "%02X%c", mac[i], (i<5)?':':' ' );
		len += sprintf( buf+len, " index:%3d  \n ", dev->ifindex ); 
		len += sprintf( buf+len, "  dev_name:%s\n ", dev->name );
		len += sprintf( buf+len, "  dev_mtu:%l\n ", dev->mtu);
		len += sprintf( buf+len, "  dev_min_mtu:%l\n ", dev->min_mtu);
		len += sprintf( buf+len, "  dev->stat.rx_packets:%l\n ", dev->stats.rx_packets);
		len += sprintf( buf+len, "  dev->stat.tx_packets:%l\n ", dev->stats.tx_packets);
		len += sprintf( buf+len, "  dev->stat.multicast:%l\n ", dev->stats.multicast);
		len += sprintf( buf+len, " irq=%d\n",dev->irq );
		len += sprintf( buf+len, "\n" );
		}
	len += sprintf( buf+len, "\n" );
	len += sprintf( buf+len, " sizeof( struct net_device )=" );
	len += sprintf( buf+len, "%d bytes\n\n", sizeof( struct net_device ) );
	return	len;
}

static const struct file_operations test_proc = 
{
        .owner = THIS_MODULE,
        .read  = my_get_info,
};



 int __init my_init( void )
{
	printk( "<1>\nInstalling \'%s\' module\n", modname );
	proc_create( modname, 0, NULL, &test_proc );
	return	0;  //SUCCESS
}

void __exit my_exit(void )
{
	remove_proc_entry( modname, NULL );
	printk( "<1>Removing \'%s\' module\n", modname );
}

module_init( my_init );
module_exit( my_exit );
MODULE_LICENSE("GPL"); 


