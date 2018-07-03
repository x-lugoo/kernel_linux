#include <stdio.h>
#include <stdlib.h>
#include "ring.h"

int main(void)
{
	struct ring_buffer rb;
	unsigned buf[10];

	ring_buffer_init(&rb,20);

	ring_buffer_write(&rb,"I have",6);
	memset(buf,0,sizeof(buf));
	ring_buffer_read(&rb,buf,4);
	printf("read buf: %s\n",buf);

	return 0;
}

