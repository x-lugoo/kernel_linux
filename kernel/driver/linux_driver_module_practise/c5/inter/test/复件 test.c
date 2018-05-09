#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>      // open() close()
#include <unistd.h>     // read() write()

#define DEVICE_NAME "/dev/2440int"


//------------------------------------- main ---------------------------------------------
int main(void) {
    int fd;
    int ret;
    char* i;
    int eint_number = 0;

    printf("\nstart gpio_led_driver test\n\n");


    fd = open(DEVICE_NAME, O_RDWR);

    printf("fd = %d\n", fd);


    if (fd == -1) {
        printf("open device %s error\n", DEVICE_NAME);
    } else {
        while (1) {
            read(fd, &eint_number, sizeof(eint_number));
            printf("the number of an jian irq is %d \n", eint_number);
            sleep(5);//等待1秒再做下一步操作


        }

        // close
        ret = close(fd);
        printf("ret=%d\n", ret);
        printf("close gpio_led_driver test\n");
    }

    return 0;
}// end main
