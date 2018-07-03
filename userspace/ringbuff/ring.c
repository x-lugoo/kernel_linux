#include "ring.h"

void ring_buffer_init(struct ring_buffer *rb,int capacity)
{
	rb->capacity = capacity;
	rb->buffer = malloc(rb->capacity);
	if(!rb->buffer){
		printf("ring buffer initialization ran out of memory\n");
		return;
	}
	rb->size = 0;
	rb->flags = 0;
	rb->head = rb->tail = 0;
}

void ring_buffer_flush(struct ring_buffer *rb)
{
	rb->size = 0;
	rb->head = rb->tail = 0;
}

void ring_buffer_set_flags(struct ring_buffer *rb,int flags)
{
	rb->flags = flags;
}

void ring_buffer_free(struct ring_buffer *rb)
{
	free(rb->buffer);
}

int __ring_buffer_write(struct ring_buffer *rb,unsigned char c)
{
	if(rb->size == rb->capacity)
		if(rb->flags & RB_FLAG_NONBLOCK)
			return 1;
		else
			while(rb->size == rb->capacity);
	rb->buffer[rb->head++] = c;
	if(rb->head == rb->capacity)
		rb->head = 0;
	rb->size++;
	return 0;
}

	

int ring_buffer_write(struct ring_buffer *rb,unsigned char *buf,int len)
{
	int rc;
	int n_wrote = 0;
	while(len--){
		rc = __ring_buffer_write(rb,*buf++);
		if(rc)
			return n_wrote;
		else 
			n_wrote++;
	}
	return n_wrote;
}

int __ring_buffer_read(struct ring_buffer *rb, int *buf)
{
    if (rb->size == 0)
        if (rb->flags & RB_FLAG_NONBLOCK)
            return 1;
        else
            while (rb->size == 0);

    *buf = rb->buffer[rb->tail++];
    if (rb->tail == rb->capacity)
        rb->tail = 0;

    rb->size--;
    return 0;
}

int ring_buffer_read(struct ring_buffer *rb, void *buf, int len)
{
    int rc;
    int n_read = 0;

    while (len--) {
        rc = __ring_buffer_read(rb, buf++);
        if (rc) return n_read;
        else n_read++;
    }

    return n_read;
}

