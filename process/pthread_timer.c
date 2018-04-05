#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/time.h>

void exit_idle(int flag);
pthread_t ntid = -1;
int count = 0;
int killflag = 0;

static pid_t gettid( void )
{
	return syscall(__NR_gettid);
}

void process_display(void *arg)
{
	 pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); 
	 pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); 
//	while(1){
		printf("count = %d,display pid=%d,ptheadid=%d\n",(int)arg,getpid(),gettid());
//	}
}


void  start_deamon_warn_idle()
{
	 int ret;
	 static int run = 0;
	 static int count = 0;

        exit_idle(1);
	if(killflag == 1 || run == 0){
	count++;
	killflag = 0;
	 run = 1;	
   	 ret = pthread_create(&ntid, NULL, process_display, (void*)count);
    	if (ret != 0)
        	printf("can't create thread:\n");
	}

		
}
void test_func()
{
	printf("test_func\n");
}

void exit_idle(int flag)
{
	int ret1,ret2;
	void *status;

	if(flag == 1){
		killflag = 1;
		return;
}

	printf("exit_idle\n");
	if(ntid != -1){
		ret1 = pthread_cancel(ntid);
		ret2 = pthread_join(ntid,&status);
		printf("\n[%d]ret=%d,ret2=%d\n",count++,ret1,ret2);
     		  if(status == PTHREAD_CANCELED){
			printf("thread has been canceled\n");
			killflag = 1;
			ntid = -1;
			}
	}
	return ;
}

void init_sigaction()
{
	struct sigaction act;
	struct sigaction alarmact;

	act.sa_handler = start_deamon_warn_idle;
	act.sa_flags  = 0;
	sigemptyset(&act.sa_mask);
	if(sigaction(SIGPROF,&act,NULL) < 0)
                printf("sigaction error sigprof\n");

        sigemptyset(&alarmact.sa_mask);
        alarmact.sa_flags = SA_SIGINFO;
        alarmact.sa_sigaction = start_deamon_warn_idle;
        if(sigaction(SIGALRM,&alarmact,NULL) < 0)
                printf("sigaction error sigalarm\n");
        

}

void init_timer(void)
{
	struct itimerval val;

	val.it_value.tv_sec = 1;
	val.it_value.tv_usec = 0;
	val.it_interval = val.it_value;
	setitimer(ITIMER_PROF,&val,NULL);
}
int main(void)
{

	start_deamon_warn_idle();
	init_sigaction();
	init_timer();
	while(1);
	return 0;
}


