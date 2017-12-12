#include<stdio.h>
#include<sys/types.h>
#include<ifaddrs.h>
#include<stdlib.h>
#include<arpa/inet.h>  

int main(void)
{
	struct ifaddrs *addr;
	struct ifaddrs *addrTmp;
	char ipAddress[INET_ADDRSTRLEN];
	struct sockaddr_in *tmp;

	memset(ipAddress,0,INET_ADDRSTRLEN);
	getifaddrs(&addr);
	addrTmp = addr;
	while(addr){
		tmp = (struct sockaddr_in*)(addr->ifa_addr);
		if((0 == strcmp(addr->ifa_name,"eth0")) && (AF_INET == tmp->sin_family))
		{
			printf("client address = %s\n", inet_ntop(AF_INET, &tmp->sin_addr, ipAddress,INET_ADDRSTRLEN));
			break;

		}	
		addr = addr->ifa_next;
	};
	freeifaddrs(addrTmp);
	return 0;
}

