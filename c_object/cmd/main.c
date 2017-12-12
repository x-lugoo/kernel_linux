#include<stdio.h>
#include"head.h"

int test(void)
{
	handle_cmd(LCD_OFF);
	handle_cmd(LCD_ON);
	handle_cmd(GPIO_HIGH);
	handle_cmd(GPIO_LOW);
	handle_cmd(9);
	return 0;
}
	
int main(void)
{
	init_gpio();
	init_lcd();
	test();
	return 0;
}

