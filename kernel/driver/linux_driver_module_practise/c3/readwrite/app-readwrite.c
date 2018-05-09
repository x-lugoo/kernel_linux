#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "stdlib.h"
#include <fcntl.h>


#define DATA_LEN 10

//#define NORMAL


int main() {
    int fd, i;

    FILE* sp;

    char bufRead[DATA_LEN];
    char bufWrite[DATA_LEN];

    int cn;

    for (i = 0; i < DATA_LEN; i++) {
        bufWrite[i] = i;
    }


    /*打开设备文件*/
    fd = open("/dev/readwrite", O_RDWR);

    if (fd < 0) {
        printf("Open chardev Error!\n");
        return -1;
    }


    bzero(bufRead, sizeof(bufRead));
    printf("dataBuf before read;\n");

    for (i = 0; i < DATA_LEN; i++) {
        printf("\n %d:%d ;", i, bufRead[i]);
    }

    printf("\n");


#ifdef NORMAL
    cn = write(fd, bufWrite, DATA_LEN);

    if (DATA_LEN != cn) {
        printf("write len err!\n");
    }

    cn = read(fd, bufRead, DATA_LEN);

#else
    cn = pwrite(fd, bufWrite, 5, 5);

    cn = pwrite(fd, bufWrite + 5, 5, 0);

    cn = pread(fd, bufRead, 7, 3);

#endif

    printf("dataBuf after read;\n");

    for (i = 0; i < DATA_LEN; i++) {
        printf("\n %d:%d ;", i, bufRead[i]);
    }

    printf("\n");

    close(fd);
    return 0;
}
