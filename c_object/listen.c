#include<stdio.h>

#define FIRE_ON (4)

struct listen
{
	int data;
	void(*listen_device)(struct listen *listen);
};

void handle_listen(struct listen *lis)
{
	lis->data = 89;
}

struct device
{
	int status;
	struct listen *li;
};

int handle_status(int status,struct device *dev)
{
	if(FIRE_ON == status)
		dev->li->listen_device(dev->li);
	return 0;
}

int main(void)
{
	struct device dev;
	struct listen listen;

	dev.li = &listen;
	listen.listen_device = handle_listen;
	
	handle_status(FIRE_ON,&dev);
	printf("the final data=%d\n",listen.data);
	return 0;
	
}
