#include<stdio.h>


void test(int input,int result) 
{ 
__asm__ __volatile__ (
	"addl %1,%0" 
	:"=r" (result)
	:"r"(input)); 

printf("result=%d\n",result);
printf("input=%d\n",input);
} 

int main(void)
{
	test(2,0);
	return 0;
}

