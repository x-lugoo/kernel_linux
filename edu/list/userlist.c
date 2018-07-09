#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> /*notice : add containof()*/
#include "list.h"

struct player_entry{
	struct list_head list;
	int player_id;
	unsigned short score;
};

static LIST_HEAD(play_head);

static int mylist_init(void)
{
	struct player_entry *tmp;
	struct player_entry *player_entry;
	int c;


	for(c = 1; c <=20;c++){
		tmp = (struct player_entry *)malloc(sizeof(struct player_entry));
		if(tmp){
			tmp->score = random();
			tmp->player_id = c;
			list_add_tail(&tmp->list,&play_head);

		}
	}
	printf("list:printing list....\n");
	list_for_each_entry(player_entry,&play_head,list){
		printf("id %d score %d\n",player_entry->player_id,player_entry->score);
	}
	return 0;
}


		
int main(void)
{
	mylist_init();
	return 0;
}

