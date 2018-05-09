#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "stdio.h"
#include "sys/types.h"
#include "sys/ioctl.h"
#include "stdlib.h"
#include "termios.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/time.h"

#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int fd;
    unsigned char buf[256];
    unsigned char temp[256];
    int i = 0;

    fd = open("/dev/rwllseek", O_RDWR);

    if (fd < 0) {
        perror("open test_driver error");
        exit(1);
    }


    for (i = 0; i < 256; i++) {
        buf[i] = i;
    }

    write(fd, buf, 256);

    //Æ«ÒÆµ½µÚ10Î»
    lseek(fd, 10, 0);

    read(fd, temp, 100);

    for (i = 0; i < 10; i++) {
        printf("%d\t", (int)temp[i]);
    }

    printf("\n");
    close(fd);

    return 0;
}

