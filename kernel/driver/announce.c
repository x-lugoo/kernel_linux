//-------------------------------------------------------------------
//	announce.c
//
//	This module sends its messages only to the current console.
//
//	NOTE: Written and tested with Linux kernel version 2.6.10.
//
//	programmer: ALLAN CRUSE
//	written on: 20 MAR 2005 
//	revised by jeff 29 NOV 2017
//-------------------------------------------------------------------

#include <linux/module.h>	// for init_module() 
#include <linux/tty.h>		// for 'struct tty_struct'
#include <linux/sched.h>

char modname[] = "announce";	// for printing module name
char msg[ 80 ];			// buffer for announcements


int init_module( void )
{
	struct tty_struct	*tty = current->signal->tty;
	int			len;
	
	len = sprintf( msg, "\nInstalling \'%s\' module \r\n\n", modname ); 
	//tty->driver->ops->write( tty, msg, len );
	tty->driver->ops->write( tty, (tty->driver->driver_name), len );

	return	0;  // SUCCESS
}

void cleanup_module( void )
{
	struct tty_struct	*tty = current->signal->tty;
	int			len;
	
	len  = sprintf( msg, "\r\nRemoving \'%s\' module. \r\n\n", modname ); 
	tty->driver->ops->write( tty, msg, len );
}

MODULE_LICENSE("GPL");

