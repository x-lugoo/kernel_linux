ifneq ($(KERNELRELEASE),)
obj-m := sync.o
else
KDIR := /usr/src/linux-2.6.24
all:
	make -C $(KDIR) M=$(PWD) modules ARCH=powerpc CROSS_COMPILE=ppc_6xx-
	chmod 777 sync.ko
	cp sync.ko /media/disk/8360/sample
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers  modul*
endif
