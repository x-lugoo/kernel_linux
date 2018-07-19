#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> /*notice : add containof()*/
#include "list.h"

#define EVALID 1 
#define EPARAM 2

struct player_entry{
	struct list_head list;
	int player_id;
	unsigned short score;
};

struct jeff {
	void (*probe)(struct jeff*);
	char *name;
};

struct jeff_parent{
	struct jeff j;
	void (*probe)(struct jeff_parent *);
	char *pname;
};

#define NONE "JEFF_NONE"

void jeff_probe(struct jeff *j)
{
	struct jeff_parent *parent;

	if(!j)
		return ;
	parent = container_of(j,struct jeff_parent,j);
	if(!parent)
		return;

	if(parent->probe){
		parent->pname = NONE;
		parent->probe(parent);
	}
}


void jeff_parent_probe(struct jeff_parent *parent)
{
	printf("jeff's parent name = %s\n",parent->pname);
}

	

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

struct jeff_parent jparent = {
	.probe = jeff_parent_probe,
	.j     = {
		.probe =  jeff_probe,
	},
};


		
int main(void)
{

//	mylist_init();
	jparent.j.probe(&jparent.j);
	return 0;
}

