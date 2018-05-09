CROSS_COMPILE	?= ppc_6xx-

obj-m := key.o

CC = $(CROSS_COMPILE)gcc

KERNELDIR = /home/xl.lin/workspace/6410/Linux/linux-2.6.38
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=arm-linux- modules 
	chmod 777 key.ko
	cp key.ko /media/disk/6410
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=arm clean
	rm -f *.ko *.mod *.o *.symvers *~ *.order

