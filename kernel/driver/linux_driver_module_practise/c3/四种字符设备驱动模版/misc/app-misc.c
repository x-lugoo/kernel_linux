#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "stdlib.h"
#include <fcntl.h>


typedef struct iotest {
    int a;
    int b;
    char c[4];
} iotest;




int g_mainthread_running = 1;

void stop_handler(int signum) {
    printf("User press Ctrl+C to shut down the service.\n");
    g_mainthread_running = 0;
    exit(0);
}



int main() {
    int fd;

    char bufRead[10];
    int cn;


    signal(SIGINT, stop_handler);


    /*打开设备文件*/
    fd = open("/dev/chardev", O_RDWR);

    if (fd < 0) {
        printf("Open chardev Error!\n");
        return -1;
    }

    iotest ioctlTest;

    while (g_mainthread_running) {
        cn = read(fd, bufRead, 2);

        if (cn == 0) {
            break;
        }

        bufRead[cn] = '\0';
        //          printf("read:%c,%c\n",bufRead[0],bufRead[1]);
        printf("read:%s\n", bufRead);
        sleep(1);

        ioctl(fd, 0, NULL);
        sleep(2);
        ioctl(fd, 1, NULL);
        sleep(2);
        ioctlTest.a = 11;
        ioctlTest.b = 22;

        ioctl(fd, 0x22, &ioctlTest);
        printf("change:c[0]:%d,c[1]:%d,c[2]:%d,c[3]:%d\n", ioctlTest.c[0],
               ioctlTest.c[1], ioctlTest.c[2], ioctlTest.c[3]);
        sleep(2);
    }

    return 0;
}
