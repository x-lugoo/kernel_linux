#ifndef __RING_H
#define __RING_H

#define RB_FLAG_NONBLOCK (1<<0) /*if set return -1 on operations that cat't do anything*/
#define RB_FLAG_SLEEP (1<<1) /*if this and NONBLOCK are set,thet the caller is put to sleep instead of busywaiting*/


struct ring_buffer{
	volatile int head; /*write pointer*/
	volatile int tail; /*read pointer*/
	int flags;      /*settings of the ring buffer*/
	volatile int capacity; /*capacity of the ring buffer*/
	volatile int size;  /*how many bytes we have already*/	
	unsigned char *buffer;/*the buffer*/
};

int ring_buffer_size(struct ring_buffer *);
void ring_buffer_init(struct ring_buffer *, int);
void ring_buffer_set_flags(struct ring_buffer *, int);
int ring_buffer_write(struct ring_buffer *, unsigned char *, int);
int ring_buffer_read(struct ring_buffer *, void *, int);
void ring_buffer_destroy(struct ring_buffer *);

#endif
