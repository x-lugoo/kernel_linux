#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/ioctl.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <sys/select.h>

#include <sys/time.h>

#include <errno.h>

int main(void)

{

    int buttons_fd;

    char buttons[6] = {'0', '0', '0', '0', '0', '0'}; //定义按键值变量，对于驱动函数中的key_values 数组

    /*打开按键设备/dev/buttons*/

    buttons_fd = open("/dev/inters", 0);

    if (buttons_fd < 0)

    {

        /*打开失败则退出*/

        perror("open device buttons");

        exit(1);

    }

    /*永读按键并打印键值和状态*/

    for (;;)

    {

        char current_buttons[6];

        int count_of_changed_key;

        int i;

        /*使用read 函数读取一组按键值（6 个）*/

        if (read(buttons_fd, current_buttons, sizeof current_buttons) != sizeof current_buttons)

        {

            perror("read buttons:");

            exit(1);

        }

        /*逐个分析读取到的按键值*/

        for (i = 0, count_of_changed_key = 0; i < sizeof buttons / sizeof buttons[0]; i++) {

            if (buttons[i] != current_buttons[i])

            {

                buttons[i] = current_buttons[i];

                /*打印按键值，并标明按键按下/抬起的状态*/

                printf("%skey %d is %s", count_of_changed_key ? ", " : "", i + 1, buttons[i] == '0' ? "up" :

                       "down");

                count_of_changed_key++;

            }

        }

        if (count_of_changed_key)

        {

            printf("\n");

        }

    }

    /*关闭按键设备文件*/

    close(buttons_fd);

    return 0;

}
