#include<stdio.h>
#include<unistd.h>


int main(void)
{
	sleep(10);
	unlink("/mnt/fs/a");
	printf("start to remove a\n");	
	return 0;
}

	
