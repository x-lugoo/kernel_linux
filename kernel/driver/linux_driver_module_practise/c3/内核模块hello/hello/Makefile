CROSS_COMPILE	?= arm-linux-

CC = $(CROSS_COMPILE)gcc
obj-m := hello.o

KERNELDIR = /opt/linux-2.6.32.2
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=arm-linux- modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) CROSS_COMPILE=arm-linux- ARCH=arm clean
	rm -f *.ko *.mod *.o *.symvers *~ *.order


