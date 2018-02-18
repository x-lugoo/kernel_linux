#include <stdio.h>		// for printf(), perror()
#include <string.h>		// for strncpy()
#include <stdlib.h>		// for exit()
#include <signal.h>		// for signal()
#include <sys/socket.h>		// for socket(), recvfrom()
#include <sys/ioctl.h>		// for SIOCGIFFLAGS, SIOCSIFFLAGS
#include <netinet/in.h>		// for htons()
#include <linux/if_ether.h>	// for ETH_P_ALL
#include <linux/if.h>		// for struct ifreq, IFNAMSIZ

#define MTU	1536		// Maximum Transfer Unit (bytes)
#define MAX_NUM	1000		// Maximum allowed packet-number

char ifname[] = "mynet0";		// name for the network interface
struct ifreq	ethreq;		// structure for 'ioctl' requests
int	sock, pkt_num;		// socket-ID and packet-number 

void my_cleanup( void )
{
	// turn off the interface's 'promiscuous' mode
	ethreq.ifr_flags &= ~IFF_PROMISC;  
	if ( ioctl( sock, SIOCSIFFLAGS, &ethreq ) < 0 )
		{ perror( "ioctl: set ifflags" ); exit(1); }
}


void my_handler( int signo ) 
{ 
	// This function executes when the user hits <CTRL-C>. 
	// It initiates program-termination, thus triggering
	// the 'cleanup' function we previously installed.
	exit(0); 
}


void display_packet( char *buf, int n )
{
	unsigned char	ch;

	printf( "\npacket #%d ", ++pkt_num );
	for (int i = 0; i < n; i+=16)
		{
		printf( "\n%04X: ", i );
		for (int j = 0; j < 16; j++)
			{
			ch = ( i + j < n ) ? buf[ i+j ] : 0;
			if ( i + j < n ) printf( "%02X ", ch );
			else	printf( "   " );
			}
		for (int j = 0; j < 16; j++)
			{
			ch = ( i + j < n ) ? buf[ i+j ] : ' ';
			if (( ch < 0x20 )||( ch > 0x7E )) ch = '.';
			printf( "%c", ch );
			}
		}
	printf( "\n%d bytes read\n-------\n", n );
}


int main( void )
{
	// create an unnamed socket for reception of ethernet packets 
	sock = socket( PF_PACKET, SOCK_RAW, htons( ETH_P_ALL ) ); 
	if ( sock < 0 ) { perror( "socket" ); exit( 1 ); }

	// enable 'promiscuous mode' for the selected socket interface
	strncpy( ethreq.ifr_name, ifname, IFNAMSIZ );
	if ( ioctl( sock, SIOCGIFFLAGS, &ethreq ) < 0 )
		{ perror( "ioctl: get ifflags" ); exit(1); }
	ethreq.ifr_flags |= IFF_PROMISC;  // enable 'promiscuous' mode
	if ( ioctl( sock, SIOCSIFFLAGS, &ethreq ) < 0 )
		{ perror( "ioctl: set ifflags" ); exit(1); }

	// make sure 'promiscuous mode' will get disabled upon termination
	atexit( my_cleanup );
	signal( SIGINT, my_handler );

	// main loop to intercept and display the ethernet packets
	char	buffer[ MTU ];
	printf( "\nMonitoring all packets on interface \'%s\' \n", ifname );
	do	{
		int	n = recvfrom( sock, buffer, MTU, 0, NULL, NULL );
		display_packet( buffer, n );
		}
	while ( pkt_num < MAX_NUM );
}

