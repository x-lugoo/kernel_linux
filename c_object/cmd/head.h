#ifndef _HEAD_H
#define _HEAD_H
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

struct stCmd
{
	int cmd;
	void(*cmd_opt)(void);
	struct stCmd *next;
};

enum cmd{
	EMPTY = 0,
	GPIO_HIGH,
	GPIO_LOW,
	LCD_ON,
	LCD_OFF,

};
extern void register_cmd(struct stCmd cmd[],int num);
extern int handle_cmd(int cmd);
extern void init_gpio(void);
extern void init_lcd(void);








#endif
