#include <stdio.h>		// for printf(), perror()
#include <string.h>		// for strncpy()
#include <stdlib.h>		// for exit()
#include <signal.h>		// for signal()
#include <unistd.h>		// for gethostname()
#include <sys/socket.h>		// for socket(), recvfrom()
#include <sys/ioctl.h>		// for SIOCGIFFLAGS, SIOCSIFFLAGS
#include <netinet/in.h>		// for htons()
#include <net/if.h>		// for 'struct ifreq', 'struct ifconf'
#include <linux/if_ether.h>	// for ETH_P_ALL
#include <linux/if_packet.h>	// for 'struct sockaddr_ll'

#define MAXPKT	0x2400		// Maximum packet-length (jumbo)
#define MAXDEV	16		// Maximum number of interfaces

struct ifconf	ifc;		// structure for ifconfig request
struct ifreq	ifr[ MAXDEV ];	// structures for interface config 
struct ifreq	ethreq;		// structure for 'ifflags' request
int	sock, pkt_num;		// for socket-ID and packet-number 
char devname[ IFNAMSIZ ];	// name for the network interface
int 	ifindex;		// interface-index for the device
int	packet_limit = 10000;	// default limit on packet-count
const char *pktype[] = { "incoming", "broadcast", "multicast", "otherhost",
			"outgoing", "loopback", "fastroute", "??" };	

void my_cleanup( void )
{
	// turn off the interface's 'promiscuous' mode
	ethreq.ifr_flags &= ~IFF_PROMISC;  
	ioctl( sock, SIOCSIFFLAGS, &ethreq ); 
}


void my_handler( int signo ) 
{ 
	// This function executes when the user hits <CTRL-C>. 
	// It initiates program-termination, thus triggering
	// the 'cleanup' function we previously installed.
	exit(0); 
}


void display_packet( char *buf, int n, int t )
{
	unsigned char	ch;

	printf( "\npacket #%d ", ++pkt_num );
	printf( " (%s) ", pktype[ t & 7 ] );
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
	printf( "\n%d bytes transferred\n-------\n", n );
}


int main( int argc, char *argv[] )
{
	// allow user to modify the default number of packets to accept
	for (int i = 1; i+1 < argc; i++)
		if ( strncmp( argv[ i ], "-c", 3 ) == 0 )
			packet_limit = atoi( argv[ i+1 ] );

	// create an unnamed socket for reception of ethernet packets 
	sock = socket( PF_PACKET, SOCK_RAW, htons( ETH_P_ALL ) ); 
	if ( sock < 0 ) { perror( "socket" ); exit( 1 ); }

	// get the list of current network interface structures
	ifc.ifc_buf = (char*)&ifr;
	ifc.ifc_len = sizeof( ifr );
	if ( ioctl( sock, SIOCGIFCONF, &ifc ) < 0 )
		{ perror( "SIOCGIFCONF" ); exit(1); }

	// obtain the interface-index for each detected device
	int	sz = sizeof( struct ifreq );
	int	dev_count = ifc.ifc_len / sz;
	for (int n = 0; n < dev_count; n++)
		if ( ioctl( sock, SIOCGIFINDEX, &ifr[ n ] ) < 0 )
			{ perror( "SIOCGIFINDEX" ); exit(1); }

	// scan command-line arguments for user's desired interface
	for (int n = 0; n < dev_count; n++)
		for (int i = 1; i < argc; i++)
			if ( strcmp( argv[i], ifr[n].ifr_name ) == 0 )
				{
				strcpy( devname, ifr[n].ifr_name );
				ifindex = ifr[n].ifr_ifindex;
				break;
				}

	// cannot continue if user omitted interface-name argument
	if ( !ifindex ) 
		{
		fprintf( stderr, "You must specify an interface: " );
		for (int n = 0; n < dev_count; n++)
			fprintf( stderr, " %s ", ifr[n].ifr_name );
		fprintf( stderr, "\n" );
		exit(1);
		}

	// enable 'promiscuous mode' for the selected socket interface
	strncpy( ethreq.ifr_name, devname, IFNAMSIZ );
	if ( ioctl( sock, SIOCGIFFLAGS, &ethreq ) < 0 )
		{ perror( "ioctl: get ifflags" ); exit(1); }
	ethreq.ifr_flags |= IFF_PROMISC;  // enable 'promiscuous' mode
	if ( ioctl( sock, SIOCSIFFLAGS, &ethreq ) < 0 )
		{ perror( "ioctl: set ifflags" ); exit(1); }

	// make sure 'promiscuous mode' will get disabled upon termination
	atexit( my_cleanup );
	signal( SIGINT, my_handler );

	// main loop to intercept and display the ethernet packets
	char	hostname[ 64 ], packet[ MAXPKT ];
	gethostname( hostname, sizeof( hostname ) );
	printf( "\nMonitoring network packets for interface \'%s\'", devname );
	printf( " on station \'%s\' \n", hostname );
	do	{
		struct sockaddr_ll	saddr = { 0 };
		socklen_t		slen = sizeof( saddr );
		size_t			plen = sizeof( packet );
		int			nbytes;

		nbytes = recvfrom( sock, packet, MAXPKT, 0, 
					(sockaddr*)&saddr, &slen );

		if ( saddr.sll_ifindex == ifindex )
			display_packet( packet, nbytes, saddr.sll_pkttype );
		}
	while ( pkt_num < packet_limit );
	return 0;
}

