#include <stdio.h>
#include <stdlib.h>

struct notifier_block{
	int (*notifier_call)(struct notifier_block *self,int);
	struct notifier_block *next;
	int priority;
	int count;
};


struct notifier_block *first;


int notifier_chain_register(struct notifier_block **list,struct notifier_block *n)
{
	while(*list){
		if(n->priority > (*list)->priority)
			break;
		list = &((*list)->next);
	}
	n->next = *list;
	*list = n;
}

int entry_notifier_chain(struct notifier_block *head)
{
	while(head){
		printf("count %d\n",head->count);
		head = head->next;
	}

	return 0;
}


int main(void)
{
	struct notifier_block *new1;
	struct notifier_block *new2;
	struct notifier_block *new3;
	struct notifier_block *new4;
	struct notifier_block *new5;

	
	first = malloc(sizeof(notifier_chain_register));
	printf("first %p\n",first);
	if(first == NULL)
		return -1;
	first->count = 0;
	first->priority = 10;
	new1 = malloc(sizeof(notifier_chain_register));
	if(new1 == NULL)
		return -1;
	new1->count = 1;
	new1->priority = 2;
	new2 = malloc(sizeof(notifier_chain_register));
	if(new2 == NULL)
		return -1;
	new2->count = 2;
	new2->priority = 4;
	new3 = malloc(sizeof(notifier_chain_register));
	if(new3 == NULL)
		return -1;
	new3->count = 3;
	new3->priority = 2;
	new4 = malloc(sizeof(notifier_chain_register));
	if(new4 == NULL)
		return -1;
	new4->count = 4;
	new4->priority = 2;
	new5 = malloc(sizeof(notifier_chain_register));
	if(new5 == NULL)
		return -1;
	new5->count = 5;
	new5->priority = 2;

	notifier_chain_register(&first,new1);
	notifier_chain_register(&first,new2);
	notifier_chain_register(&first,new3);
	notifier_chain_register(&first,new4);
	notifier_chain_register(&first,new5);

	entry_notifier_chain(first);
	return 0;
}


