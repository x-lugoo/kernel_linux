#include<stdio.h>
#include"head.h"
static struct stCmd *pscmd = NULL;

void register_cmd(struct stCmd cmd[],int num)
{
	int i = 0;
	struct stCmd **tmp;

	for(;i < num;i++){
		tmp = &pscmd;	
		while(*tmp){
			tmp = &(*tmp)->next;
		}
		*tmp = &cmd[i];				
	}
}

int handle_cmd(int cmd)
{
	struct stCmd *tmp;

	for(tmp = pscmd;tmp;tmp=tmp->next){
		if(tmp->cmd == cmd){
			tmp->cmd_opt();
			return 0;
		}
	}
	printf("can't find matched cmd %d\n",cmd);
	return -1;
}


