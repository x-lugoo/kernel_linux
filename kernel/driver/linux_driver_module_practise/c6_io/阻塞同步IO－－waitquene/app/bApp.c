#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {
    int flag, fd, result;

    flag = atoi(argv[1]);

    char buf[10];

    fd = open("/dev/brock", O_RDWR);

    if (fd < 0) {
        printf("open erro!\n");
    }

    if (flag == 1) { /* read */
        printf(" read.......  \n ");
        read(fd, buf, 4);
    } else {
        printf("write......... \n");
        result = write(fd, buf, 4);
        printf("write cnt :%d\n", result);
    }

    close(fd);
}
