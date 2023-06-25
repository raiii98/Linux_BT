# Hello world kernel module

EXTRA_CFLAGS = -Wall
obj-m = ex1.o
CC:= gcc
FLAG_THREAD:= -lpthread
KDIR = /lib/modules/`uname -r`/build

all:
	make -C $(KDIR) M=`pwd` modules

prog:
	$(CC) -o example prog.c $(FLAG_THREAD)
run:
	sudo rmmod ex1 && sudo insmod ex1.ko && sudo chmod 0777 /dev/my_device
clean:
	make -C $(KDIR) M=`pwd` clean