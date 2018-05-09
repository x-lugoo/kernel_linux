#include <stdio.h>
#include "stdlib.h"
#include <stdint.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <memory.h>
#include <linux/input.h>
#include "key_app.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>


#define IOCTL_READKEY  0x01


int g_mainthread_running = 0; /* 程序运行标记 */


/*******************************************************************
 * * 函数名称: stop_handler
 * *
 * * 功能描述: 对异常信号Ctrl + C 的响应函数
 * *
 * * 输入参数: 异常信号
 * *
 * * 输出参数: 无
 * *
 * * 说明    :  无
 * ********************************************************************/
void stop_handler(int signum) {
    printf("User press Ctrl+C to shut down the service.\n");
    g_mainthread_running = 0;
    exit(0);
}


int main(int argc, char** argv) {
    int keyFd, miFd;
    struct input_event ev_key;
    int key_value, i = 0, count;

    signal(SIGINT, stop_handler);

    g_mainthread_running = 1;

    printf("----->\n");

    keyFd = open("/dev/event1", O_RDWR);

    if (keyFd <= 0) {
        printf("cannot open /dev/key\n");
        return 0;
    } else {
        printf("open /dev/key ok\n");
    }


    while (g_mainthread_running) {


        count = read(keyFd, &ev_key, sizeof(struct input_event));

        for (i = 0; i < (int)count / sizeof(struct input_event); i++) {
            if (EV_KEY == ev_key.type) {
                printf("type:%d,code:%d,value:%d\n", ev_key.type, ev_key.code, ev_key.value);
            } else {
                printf("type:%d\n", ev_key.type);
            }
        }

        if (EV_SYN == ev_key.type) {
            printf("syn event\n\n");
        }

    }

    return 0;
}
