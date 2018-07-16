#include <stdio.h>
#include <stdlib.h>

#define EFAULT (1<<0)
#define ENOMEM (1<<1)


struct module{
	char *name;
	unsigned int flags;
	struct module *next;
	void(*cleanup)(struct module *m);
};

static struct module *module_list;

struct module *find_module(const char *name)
{
	struct module *list;

	for(list= module_list;list;list=list->next){
		if(!strcmp(list->name,name))
			break;
	}
	return list;
}

int insert_module(struct module *mod)
{
	if(!mod)
		return -EFAULT;
	mod->next = module_list;
	module_list = mod;
	return 0;
}
	
int init_module(void)
{
	int ret;

	struct module *m1 = (struct module*)malloc(sizeof(struct module));
	if(!m1){
		ret = -ENOMEM;
		goto err1;
	}
	m1->name = "module1";
	struct module *m2 = (struct module*)malloc(sizeof(struct module));
	if(!m2){
		ret = -ENOMEM;
		goto err1;
	}
	m2->name = "module2";
	struct module *m3 = (struct module*)malloc(sizeof(struct module));
	if(!m2){
		ret = -ENOMEM;
		goto err1;
	}
	m3->name = "module3";
	ret = insert_module(m1);
	if(ret)
		goto err1;
	ret = insert_module(m2);
	if(ret)
		goto err1;
	ret = insert_module(m3);
	if(ret)
		goto err1;
	return 0;
err1:
	return ret;
}



int main(void)
{
	int ret;
	struct module *tmp;

	ret = init_module();
	if(ret)
		return ret;
	tmp = find_module("module2");
	if(tmp)
		printf("name %s\n",tmp->name);
	return 0;
}

