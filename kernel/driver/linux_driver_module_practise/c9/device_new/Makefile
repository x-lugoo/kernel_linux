ifneq ($(KERNELRELEASE),)
obj-m := device_new_2.o
else
KDIR := /lib/modules/$(shell uname -r)/build 
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers  modul*
endif
