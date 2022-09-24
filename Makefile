VERSION = 5
PATCHLEVEL = 15
SUBLEVEL = 0

obj-m += kernKill.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
