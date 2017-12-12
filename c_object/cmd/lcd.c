#include<stdio.h>
#include"head.h"

static void lcd_on(void)
{
	printf("lcd on\n");
}

static void lcd_off(void)
{
	printf("lcd off\n");
}

struct stCmd lcdCmd[]={
	{LCD_ON,lcd_on,NULL},
	{LCD_OFF,lcd_off,NULL}
};

void init_lcd(void)
{
	register_cmd(lcdCmd,ARRAY_SIZE(lcdCmd));
}


	
