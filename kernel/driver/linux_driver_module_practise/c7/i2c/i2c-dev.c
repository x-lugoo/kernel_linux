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
        printf("Usage:\n%s /dev/i2c-x start_addr reg_addr value\n", argv[0]);
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

    e2prom_data.nmsgs = 2;
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
    e2prom_data.msgs[0].len = 2;
    e2prom_data.msgs[0].addr = slave_address;
    e2prom_data.msgs[0].flags = 0;//写命令
    e2prom_data.msgs[0].buf = (unsigned char*)malloc(2);
    e2prom_data.msgs[0].buf[0] = reg_address;
    e2prom_data.msgs[0].buf[1] = value;

    ret = ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data);

    if (ret < 0) {
        printf("ioctl write error\n");
    }

    printf("you have write %02x into e2prom at %02x address\n", value, reg_address);

    sleep(1);
    /*read data from e2prom*/
    e2prom_data.nmsgs = 2;
    e2prom_data.msgs[0].len = 1;
    e2prom_data.msgs[0].addr = slave_address;
    e2prom_data.msgs[0].flags = 0;//写命令
    e2prom_data.msgs[0].buf[0] = reg_address;

    e2prom_data.msgs[1].len = 1;
    e2prom_data.msgs[1].addr = slave_address;
    e2prom_data.msgs[1].flags = I2C_M_RD;//读命令
    e2prom_data.msgs[1].buf = (unsigned char*)malloc(1);
    e2prom_data.msgs[1].buf[0] = 0;
    ret = ioctl(fd, I2C_RDWR, (unsigned long)&e2prom_data);

    if (ret < 0) {
        printf("ioctl read error\n");
    }

    printf("read %02x from e2prom address %02x\n", e2prom_data.msgs[1].buf[0], reg_address);

    close(fd);
    return 0;
}