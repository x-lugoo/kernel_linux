#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/list.h>

struct player_entry{
	int player_id;
	unsigned short score;
	struct list_head list;
};

struct player_entry play_head;
static int __init mylist_init(void)
{
	struct player_entry *tmp;
	struct list_head *cur;
	int c;

	INIT_LIST_HEAD(&play_head.list);

	for(c = 1;c <= 20;c++){
		tmp = (struct player_entry*)kmalloc(sizeof(struct player_entry),GFP_KERNEL);
		if(tmp){
			get_random_bytes(&tmp->score,sizeof(unsigned short));
			tmp->player_id = c;
			printk(KERN_INFO "list:add player %d with score %d\n",
					tmp->player_id,tmp->score);
			list_add_tail(&tmp->list,&play_head.list);
		}
	}
	printk(KERN_INFO "list: printing list...\n");
	list_for_each(cur,&play_head.list){
		tmp = list_entry(cur,struct player_entry,list);
		printk(KERN_INFO "id %d  score %d\n",tmp->player_id,tmp->score);
	}
	return 0;
}

static void __exit mylist_exit(void)
{

	struct player_entry *tmp;
	struct  list_head *cur,*q;

	printk(KERN_INFO "list:%s\n",__FUNCTION__);
	list_for_each_safe(cur,q,&play_head.list){
		tmp = list_entry(cur,struct player_entry,list);
		printk(KERN_INFO "list :deleting player %d\n",tmp->player_id);
		list_del(cur);
		kfree(tmp);
	}
}

module_init(mylist_init);
module_exit(mylist_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("JEFF");


	

