obj-m := key.o

KERNELDIR = /opt/linux-2.6.32.2
PWD := $(shell pwd)

default:
	make -C $(KERNELDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=arm-linux- modules 
clean:
	rm -f *.ko *.mod *.o *.symvers *~ *.order

