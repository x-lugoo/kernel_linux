#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <time.h>

#define MEMDEV_SIZE 4096

int main(void) {
    int fd1, fd2, maxfdp, ret;
    fd_set fds;
    struct timeval tv;
    char buf[MEMDEV_SIZE];

    if (-1 == (fd1 = open("/dev/polldev", O_RDWR))) {
        printf("open dev0 error\n");
        _exit(EXIT_FAILURE);
    }

    FD_ZERO(&fds);
    //设置要监控的文件描述符
    FD_SET(fd1, &fds);
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    printf("Test for select(): ...\n");

    while (1) {
        printf(" ==> select ...\n");

        if (-1 == (ret = select(fd1 + 1, &fds, NULL, NULL, &tv))) {
            printf("select error\n");
            _exit(EXIT_FAILURE);
        }

        if (0 == ret) {
            printf("No data within 10s, exit\n");
            break;
        } else {

            FD_ZERO(&fds);
            FD_SET(fd1, &fds);
        }
    }

    _exit(EXIT_SUCCESS);
}
