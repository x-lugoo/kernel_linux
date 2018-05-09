#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>


typedef struct student {
    char name[100];
    int num;
    struct list_head list;
} student;

student* pstudent;
student* tmp;

struct list_head student_list;
struct list_head* pos;


void listExport(void) {
    list_for_each(pos, &student_list) {
        tmp = list_entry(pos, student, list);
        printk("list-->student %d name: %s\n", tmp->num, tmp->name);
    }
}

EXPORT_SYMBOL(listExport);

int listInit(void) {
    int i = 0;

    INIT_LIST_HEAD(&student_list);

    pstudent = kmalloc(sizeof(student) * 5, GFP_KERNEL);
    memset(pstudent, 0, sizeof(student) * 5);

    for (i = 0; i < 5; i++) {
        sprintf(pstudent[i].name, "Student%d", i + 1);
        pstudent[i].num = i + 1;
        list_add(&(pstudent[i].list), &student_list);
    }


    list_for_each(pos, &student_list) {
        tmp = list_entry(pos, student, list);
        printk("--->student %d name: %s\n", tmp->num, tmp->name);
    }

    return 0;
}


void listExit(void) {
    int i ;

    for (i = 0; i < 5; i++) {
        list_del(&(pstudent[i].list));
    }

    kfree(pstudent);
}

module_init(listInit);
module_exit(listExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lxlong");
MODULE_DESCRIPTION("List Module");

