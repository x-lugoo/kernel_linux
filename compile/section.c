#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

typedef void(*init_call)(void);


extern init_call _jinit_start;
extern init_call _jinit_end;

#define _init __attribute__((unused,section(".myinit")))
#define DECLARE_INIT(func) init_call _fn_##func _init=func

static  void A_init(void)
{
	printf("%s\n",__func__);
}

DECLARE_INIT(A_init);


static  void B_init(void)
{
	printf("%s\n",__func__);
}

DECLARE_INIT(B_init);


static  void C_init(void)
{
	printf("%s\n",__func__);
}

DECLARE_INIT(C_init);

void do_initcalls(void)
{
	init_call* init_ptr = &_jinit_start;
	printf("A_init=%p B_init=%p C_init=%p \n",A_init,B_init,C_init);
	printf("_init_start=%p _init_end=%p %s\n",_jinit_start,_jinit_end,__func__);
		A_init();
		(*init_ptr)();
	for(;init_ptr < &_jinit_end;init_ptr++){
		printf("init address =%p\n",init_ptr);
		(*init_ptr)();
	}

}

int main(void)
{
	do_initcalls();
	return 0;
}









