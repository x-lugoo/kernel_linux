//-------------------------------------------------------------------
//	mmake.cpp	    (An LKM developer's tool for Linux 2.6) 
//
//	This utility compiles the kernel-modules whose source-files
//	reside in the current directory (then cleans up afterward).
//	
//	Adapted from: "Building and Running Modules" (Chapter Two), 
//	in: "Linux Device Drivers (3rd Ed)," by Rubini and Corbett. 
//
//	programmer: ALLAN CRUSE
//	written on: 22 JAN 2005
//	revised on: 19 MAR 2005 -- command-line arguments override
//	revised on: 27 JUL 2006 -- extra cleanup for kernel 2.6.17
//	revised on: 31 MAY 2007 -- adjust extra cleanup for 2.6.18
//	revised on: 19 JUN 2008 -- improve cleanup step for 2.6.25
//	revised on: 19 JUL 2008 -- employ extra cleanup for 2.6.26
//	reviser on: 01 DEC 2017 -- etc.,
//-------------------------------------------------------------------

#include <stdio.h>	// for fopen(), fprintf(), fclose(), perror() 
#include <dirent.h>	// for opendir(), readdir()
#include <string.h>	// for strchr(), strncmp(), strncpy()
#include <stdlib.h>	// for exit() 

int main( int argc, char **argv )
{
	DIR	*dp = opendir( "." );	// open the current directory
	if ( dp == NULL ) { perror( "opendir" ); exit(1); }

	FILE	*fp = fopen( "Makefile", "w" );	// create 'Makefile'
	if ( fp == NULL ) { perror( "Makefile" ); exit(1); }

	// write boilerplate statements to compile Linux 2.6 modules	
	fprintf( fp, "\nifneq\t($(KERNELRELEASE),)" );
	fprintf( fp, "\nobj-m\t:= " ); 

	// form the list of ".o" files from command-line arguments
	char	filename[ NAME_MAX ];
	for (int i = 1; i < argc; i++)
		{
		strncpy( filename, argv[i], NAME_MAX );
		char	*suffixp = strchr( filename, '.' );
		if (( suffixp == NULL )&&( strlen(filename) < NAME_MAX-3 ))
			strncat( filename, ".o", 3 );  
		suffixp = strchr( filename, '.' );
		if ( strncmp( suffixp, ".o", 3 ) != 0 ) 
			strncpy( suffixp, ".o", 3 );
		fprintf( fp, "%s ", filename );
		}

	// or form the list of ".o" files from current ".c" files	
	struct dirent		*entry;
	if ( argc == 1 ) while ( entry = readdir( dp ) )
		{
		strncpy( filename, entry->d_name, NAME_MAX );
		char	*suffixp = strchr( filename, '.' );
		if ( suffixp == NULL ) continue;
		if ( strncmp( suffixp, ".c", 3 ) != 0 ) continue; 
		strncpy( suffixp, ".o", 3 );
		fprintf( fp, "%s ", filename );
		}

	// generate the remainder of the 'Makefile' boilerplate 
	fprintf( fp, "\n" );
	fprintf( fp, "\nelse" );
	fprintf( fp, "\nKDIR\t:= /lib/modules/$(shell uname -r)/build" );
	fprintf( fp, "\nPWD\t:= $(shell pwd)" );
	fprintf( fp, "\n" );
	fprintf( fp, "default:\t" );
	fprintf( fp, "\n\t$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules ");
	fprintf( fp, "\n\trm -r -f %s ", filename  );
	fprintf( fp, "\nendif" );
	fprintf( fp, "\n\n" );
	fclose( fp );

	// execute the 'make' utility using our just-created 'Makefile'
	system(" make  > /dev/null  ");
	
	// now clean up the file-clutter left in this subdirectory
	system(" rm -r -f .tmp_versions " );
	system(" rm -r -f .*.cmd " );
	system(" rm -r -f *.mod.c " );
	system(" rm -r -f *.mod.o " );
	system(" rm -r -f *.symvers ");
	system(" rm -r -f *.order ");
	system(" rm -r -f *.o.d " );
	system(" rm -r -f *.markers ");
	system(" rm -r -f Makefile ");
}
