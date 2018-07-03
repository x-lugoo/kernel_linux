#include <stdio.h>

#define MODULE_NAME TEST
#define ___stringify(x) #x
#define __stringify(x) ___stringify(x)
#define __MODULE_NAME __stringify(MODULE_NAME) ":"
#define printk(fmt,...) printf(__MODULE_NAME "%s(%d)" fmt,__func__,__LINE__,##__VA_ARGS__);


int main(void)
{
	printk("jeff\n");
	return 0;
}

