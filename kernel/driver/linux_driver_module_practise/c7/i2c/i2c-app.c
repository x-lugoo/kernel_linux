#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define DATA_RW  100

int main(int argc, char** argv) {
    int i, fd;
    char dataWrite[DATA_RW];
    char dataRead[DATA_RW];

    fd = open("/dev/at24c08b", O_RDWR);

    if (fd < 0) {
        printf("Open at24c08b Device Faild!\n");
        exit(1);
    }

    for (i = 0; i < DATA_RW; i++) {
        dataWrite[i] = i;
    }

    write(fd, dataWrite, DATA_RW);

    sleep(1);

    memset(dataRead, 0, sizeof(dataRead));
    read(fd, dataRead, DATA_RW);

    for (i = 0; i < DATA_RW; i++) {
        if (dataRead[i]  != dataWrite[i]) {
            break;
        }
    }

    if (i < DATA_RW) {
        printf("read write wrong,index=%d\n", i);
    } else {
        printf("eeprom read write the same!\n");
    }

    close(fd);
    return 0;
}
