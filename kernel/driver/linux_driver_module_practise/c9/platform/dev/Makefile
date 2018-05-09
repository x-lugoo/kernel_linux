ifneq ($(KERNELRELEASE),)
obj-m := device.o
else
KDIR := /opt/linux-2.6.32.2
all:
	make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-
	chmod 777 device.ko
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers  modul*
endif
