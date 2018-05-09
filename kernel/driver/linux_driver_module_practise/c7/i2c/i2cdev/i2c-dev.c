#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

int main(int argc, char** argv) {
    struct i2c_rdwr_ioctl_data e2prom_data;
    unsigned int fd;
    unsigned int slave_address, reg_address, value; //slave_address为eeprom的地址，reg_address为eeprom中存储单元的地址，范围0x0--0xFFFFFFFF,value为你要写进eeprom的值
    int ret;

    if (argc < 5) {
        printf("Usage:\n%s /dev/i2c/x start_addr reg_addr value\n", argv[0]);
        return 0;
    }

    fd = open(argv[1], O_RDWR);

    if (!fd) {
        printf("Error on opening the device file\n");
        return 0;
    }

    sscanf(argv[2], "%x", &slave_address);
    sscanf(argv[3], "%x", &reg_address);
    sscanf(argv[4], "%x", &value);

    e2prom_data.nmsgs = 2;//因为都时序要两次，所以设为2
    e2prom_data.msgs = (struct i2c_msg*)malloc(e2prom_data.nmsgs * sizeof(struct i2c_msg));

    if (!e2prom_data.msgs) {
        printf("Memory alloc error\n");
        close(fd);
        return 0;
    }

    ioctl(fd, I2C_TIMEOUT, 2);//设置超时时间
    ioctl(fd, I2C_RETRIES, 1);//设置重发次数

    /* write data to e2prom*/
    e2prom_data.nmsgs = 1;
    e2prom_data.msgs[0].len = 2;//信息长度为2，看写时序，eeprom的地址不算的，因为付给了addr，而len是指buf中的值的个数
    e2prom_data.msgs[0].addr = slave_address;
    e2prom_data.msgs[0].flags = 0;//写命令
    e2prom_data.msgs[0].buf = (unsigned char*)malloc(2);
    e2prom_data.msgs[0].buf[0] = reg_address;//信息值1 eeprom中存储单元的地址，即你要往哪写
    e2prom_data.msgs[0].buf[1] = value;//信息值2,即你要写什么

    ret = ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); //好了 ，写进去吧

    if (ret < 0) {
        printf("ioctl write error\n");
    }

    printf("you have write %02x into e2prom at %02x address\n", value, reg_address);

    sleep(1);
    /*read data from e2prom*/
    e2prom_data.nmsgs = 2;//读时序要两次过程，要发两次I2C消息
    e2prom_data.msgs[0].len = 1;//信息长度为1，第一次只写要读的eeprom中存储单元的地址
    e2prom_data.msgs[0].addr = slave_address;
    e2prom_data.msgs[0].flags = 0;//写命令，看读时序理解
    e2prom_data.msgs[0].buf[0] = reg_address;//信息值

    e2prom_data.msgs[1].len = 1;
    e2prom_data.msgs[1].addr = slave_address;
    e2prom_data.msgs[1].flags = I2C_M_RD;//读命令
    e2prom_data.msgs[1].buf = (unsigned char*)malloc(1);
    e2prom_data.msgs[1].buf[0] = 0;//先清空要读的缓冲区
    ret = ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data); //好了，读吧

    if (ret < 0) {
        printf("ioctl read error\n");
    }

    printf("read %02x from e2prom address %02x\n", e2prom_data.msgs[1].buf[0], reg_address);

    close(fd);
    return 0;
}