#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "stdlib.h"
#include <fcntl.h>

int g_mainthread_running = 1;

void stop_handler(int signum) {
    printf("User press Ctrl+C to shut down the service.\n");
    g_mainthread_running = 0;
    exit(0);
}



int main(int argc, char** argv) {
    int fp0;
    char temp[10];
    int led_num = 0;
    int led_value = 0;

    strcpy(temp, argv[1]);
    led_num = atoi(temp);


    signal(SIGINT, stop_handler);


    /*打开设备文件*/
    fp0 = open("/dev/leddev", O_RDWR);

    if (fp0 < 0) {
        printf("Open leddev Error!\n");
        return -1;
    }



    printf("%d %d\n", led_num, led_value);





    while (g_mainthread_running) {
        ioctl(fp0, 0, led_num);

        sleep(1);
        ioctl(fp0, 1, led_num);
        sleep(1);
    }

    return 0;
}
