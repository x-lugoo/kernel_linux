/*函数功能：实现4个LED灯的同时亮灭，间隔为1秒*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#define DEV_NAME "/dev/mmapled"

int main() {
    int fd, k;
    void* start, *reg = NULL;

    /* fd = open(DEV_NAME,O_RDWR);

    */

    if ((fd = open("/dev/mem", O_RDWR)) < 0) {
        printf("Open mem dev failed!\n");
        return -1;
    }


    if (fd < 0) {
        printf("Open device err!\n");
        return -1;
    }

    /*参数解释:
     * NULL:映射到的内核虚拟地址,设置为NULL由内核决定
     * 1024*4:映射大小,最小一页,必为页大小的整数倍
     * 映射区的权限
     * 对映射区的修改改变映射区的内容
     * fd:open返回的文件描述符
     * 物理地址,一个页的起始物理地址,它>>PAGE_SHIFT之后传给驱动的vma结构体的vm_pgoff
     */
    /*0x56000000是LED等所在的GPIO口的BANK起始物理地址*/
    /*start是的得到的虚拟地址*/
    start = mmap(NULL, 1024 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x56000000);

    if (start == NULL) {
        printf("mmap err!\n");
        return -1;
    }

    reg = start + 0x10;            //GPBCON，控制寄存器
    *(unsigned*)reg &= 0xfffc03ff; //[17:10]清零
    *(unsigned*)reg |= 0x00015400; //[17:10]=01010101,输出功能
    reg = start + 0x14;            //GPBDAT

    /*量灭k次，实现对LED的操作*/
    k = 25;

    while (k--) {
        *(unsigned*)reg &= ~(0x1e0); //[8:5],set 0,led on
        sleep(1);
        *(unsigned*)reg |= 0x1e0;    //[8:5],set 1,led off
        sleep(1);
        printf("k= %d\n", k);
    }

    /*取消映射*/
    munmap(start, 1024 * 4);
    close(fd);
    return 0;
}