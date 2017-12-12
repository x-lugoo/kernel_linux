//-------------------------------------------------------------------
//	mmfork.cpp
//
//	Examines the Linux task_struct's Memory Manager structure 
//	for a parent-process and its child by reading kernel data 
//	provided by a pseudo-file (created by our 'mm.c' module).
//
//	programmer: ALLAN CRUSE
//	written on: 21 SEP 2004
//-------------------------------------------------------------------

#include <stdio.h>	// for printf(), perror() 
#include <fcntl.h>	// for open() 
#include <stdlib.h>	// for exit() 
#include <unistd.h>	// for read(), write(), close() 
#include <sys/wait.h>	// for wait()

void show_task_mm_struct_info( void )
{
	// open the pseudo-file
	char filename[] = "/proc/mm";
	int	fd = open( filename, O_RDONLY );
	if ( fd < 0 ) { perror( filename ); exit(1); }
	
	// read and display this file's contents
	char	buffer[ 1024 ];
	int	nbytes = read( fd, buffer, sizeof( buffer) );
	if ( nbytes < 0 ) { perror( "read" ); exit(1); }
	else	write( 1, buffer, nbytes );

	// close the pseudo-file 
	close( fd );
}

int main( int argc, char **argv )
{

	// fork a child-process
	int	pid = fork();
	if ( pid == 0 )
		{  // child-process
		malloc(1024);
		show_task_mm_struct_info();
		printf( "child-process is quitting\n" );
		exit(0);
		}
	else	{  // parent-process 
		int	status = 0;
		wait( &status ); // wait until child exits
		show_task_mm_struct_info();	
		printf( "parent-process is quitting\n\n" );	
		}
	
	// remove the kernel module
	system( "/sbin/rmmod mm" );	
}
