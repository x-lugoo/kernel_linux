#include<stdio.h>
#include<stdlib.h>

struct ppoint_struct {
	int data;
	struct ppoint_struct *next;
};

static struct ppoint_struct *first = NULL;

void *insert_data(struct ppoint_struct *data)
{
	struct ppoint_struct **p;
	
	p = &first;
	while(*p){
		p = &(*p)->next;
	};
	*p = data;
	return NULL;
}

void *del_data(int data)
{
	struct ppoint_struct **p;
	struct ppoint_struct *tmp;

	p = &first;	
	while((tmp = *p)){
		if(tmp->data != data){
			p = &tmp->next;
		}
		else{
			break;
		}
	}
	*p = tmp->next;
	free(tmp);
	return NULL;
}

void lookup_data(void)
{
	struct ppoint_struct *tmp;
	
	for(tmp = first;tmp;tmp = tmp->next){
		printf("data=%d\n",tmp->data);
	}
}

int main(void)
{
	struct ppoint_struct *p1;	
	struct ppoint_struct *p2;	
	struct ppoint_struct *p3;	
	struct ppoint_struct *p4;	
	struct ppoint_struct *p5;	

	p1 = malloc(sizeof(*p1));
	p2 = malloc(sizeof(*p1));
	p3 = malloc(sizeof(*p1));
	p4 = malloc(sizeof(*p1));
	p5 = malloc(sizeof(*p1));
	p1->data = 1;
	p2->data = 2;
	p3->data = 3;
	p4->data = 4;
	p5->data = 5;
	insert_data(p1);
	insert_data(p2);
	insert_data(p3);
	insert_data(p4);
	printf("inserted data 1,2,3,4\n");
	lookup_data();
	del_data(3);
	printf("del data 3\n");
	lookup_data();
	insert_data(p5);	
	printf("inserted data 5\n");
	lookup_data();
	return 0;
}

