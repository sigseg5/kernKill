VERSION = 6
PATCHLEVEL = 5
SUBLEVEL = 6

# Default values
USB_VENDOR_ID ?= 0x18a5
USB_PRODUCT_ID ?= 0x0302

obj-m += kern_kill.o

ccflags-y += -fanalyzer -std=gnu11 -Wall -Werror
ccflags-y += -DUSB_VENDOR_ID=$(USB_VENDOR_ID) -DUSB_PRODUCT_ID=$(USB_PRODUCT_ID)

ifeq ($(MODE),reboot)
ccflags-y += -DMODE_REBOOT
else
ccflags-y += -DMODE_KILL
endif


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
