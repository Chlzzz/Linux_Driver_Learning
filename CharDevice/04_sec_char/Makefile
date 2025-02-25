obj-m += SecCharDev.o

KDIR:=/usr/src/linux-headers-5.15.0-131-generic

PWD?=$(shell pwd)

all:
	make -C ${KDIR} M=${PWD} modules

clean:	
	rm -f *.ko *.o *.mod *.mod.o *.mod.c *.symvers *.order
