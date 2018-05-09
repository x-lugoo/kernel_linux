#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char** argv) {
    int i, fd;
    unsigned int value[512];

    value[0] = 0x55;
    value[1] = 0xaa;
    value[2] = 0x55;
    value[3] = 0xaa;
    value[4] = 0xaa;
    value[5] = 0x55;

    fd = open("/dev/at24c08b", O_RDWR);

    if (fd < 0) {
        printf("Open at24c08b Device Faild!\n");
        exit(1);
    }

    write(fd, value, 6);

    for (i = 0; i < 6; i++) {
        printf("write reg[%d] data: %x to at24c08\n", i, value[i]);
    }

    printf("#########################################\n");
    sleep(1);
    read(fd, value, 6);

    for (i = 0; i < 6; i++) {
        printf("read reg[%d] data: %x to at24c08\n", i, value[i]);
    }

    close(fd);
    return 0;
}
