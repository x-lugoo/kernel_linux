#include<stdio.h>
#include"head.h"

static void gpio_high(void)
{
	printf("gpio_high\n");
}

static void gpio_low(void)
{
	printf("gpio_low\n");
}

struct stCmd gpioCmd[]={
	{GPIO_HIGH,gpio_high,NULL},
	{GPIO_LOW,gpio_low,NULL}
};

void init_gpio(void)
{
	register_cmd(gpioCmd,ARRAY_SIZE(gpioCmd));
}


	
