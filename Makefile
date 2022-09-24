VERSION = 5
PATCHLEVEL = 15
SUBLEVEL = 0

obj-m += kernKill.o
ccflags-y += -fanalyzer -std=gnu11 -Wall #-pedantic

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
