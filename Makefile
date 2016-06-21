obj-m+=kobj.o
kobj-objs := test.o dev.o
CURRENT_PATH:=$(shell pwd)

VERSION_NUM:=$(shell uname -r)

#LINUX_PATH:=/home/test/kernels/linux-3.14.66
LINUX_PATH:=/usr/src/linux-headers-4.2.0-35-generic

all:
	$(MAKE) -C $(LINUX_PATH) M=$(PWD) modules

clean:
	rm *.mod.c *.o *.order *.symvers *.ko
