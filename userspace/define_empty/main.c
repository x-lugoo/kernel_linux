#include <stdio.h>

#define BT

typedef int bt_t;

struct data{
	int tools;
	int skill;
};


struct jeff{
	char *name;
	void (*make_memory)(struct jeff *);
#ifdef BT
	void (*bt)(int);
#endif
	void (*init)(struct jeff*,struct data*);
	bt_t  (*study_bt)(struct jeff*);
	void *pri_data;
	void (*probe)(struct jeff*);
	int generalskill;
};

struct jeff_parent{
	void (*probe)(struct jeff_parent *,int );
	struct jeff j;
};

bt_t general_study_bt(struct jeff * study)
{
	printf("call general study bt, study skill %d \n",study->generalskill);
	return 0;
}


void jeff_make_memory(struct jeff *memory)
{
	struct data *data = memory->pri_data;
	printf("made %d memory\n",data->tools * data->skill);
	return ;
}

void jeff_init(struct jeff *init,struct data *data)
{
	init->pri_data = data;
}


#ifdef BT

void bt(int i)
{
	printf("i %d\n",i);
}
#else

#define bt NULL

#endif

bt_t jeff_study_bt(struct jeff *study)
{

	return 0;
}

#define instance_jeff_profile(instance) \
	struct jeff instance = { \
	.name = "jeff",  \
	.make_memory = jeff_make_memory, \
	.study_bt   = jeff_study_bt, \
	.init = jeff_init, \
};

instance_jeff_profile(instance);

struct data d = {
	.tools = 2,
	.skill = 50,
};

int main(void)
{

	if(instance.init)
		instance.init(&instance,&d);

	instance.make_memory(&instance);
	if(instance.study_bt)
		instance.study_bt(&instance);
	else
		general_study_bt(&instance);

	return 0;

}

