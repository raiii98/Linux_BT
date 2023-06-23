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
clean:
	make -C $(KDIR) M=`pwd` clean