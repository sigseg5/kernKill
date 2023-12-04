# kernKill

kernKill is a work-in-progress Linux kernel loadable module designed for emergency system management. It's primary function is to enable immediate shutdown or termination specific processes when a specified USB device is removed from a machine running a compatible Linux operating system.

## Warning about the USB Stack

It's important to be aware of the nuances and potential risks associated with the USB stack when using kernKill. Detailed information about these aspects is provided in the following sections.

## Configuration process

To configure kernKill, you need to know some system information about your system's USB devices. You can gather this information using the following commands:

```bash
# List all connected USB devices
lsusb

# Display driver information for USB devices
lsusb -t
```

You'll need to note USB Vendor and Product IDs. Next you should set these as environment variables with `export ...` or pass IDs like `USB_PRODUCT_ID=0xXXXX USB_VENDOR_ID=XXXX ...`. If you don't pass any IDs, the default values from the [Makefile](/Makefile) will be used.

## Building the module

### About build modes

There are two modes:

- `kill` (default) – in this mode, processes specified in [proc_list.h](/proc_list.h) are immediately killed from kernel space via SIGKILL.
- `reboot` – in this mode, the kernel module will perform `emergency_restart()` exec from kernel space.

kernKill is configurable via the [Makefile](/Makefile) included in the source code. By default, it operates in 'kill' mode, but you can switch to 'reboot' mode if necessary. Here's how to build in each mode:

```bash
# Building in 'kill' mode (default)
USB_PRODUCT_ID=0x5678 \
    USB_VENDOR_ID=0x1234 \
    make

# Building in 'reboot' mode
USB_PRODUCT_ID=0x5678 \
    USB_VENDOR_ID=0x1234 \
    make MODE=reboot
```

## Installation guide

To install kernKill, run the provided script. This installation will also disable the `uas` and `usb-storage` drivers. This is to prevent the standard usb-storage stack from taking control of the USB device. In fact, it is enough to write a udev-rule to achieve feature parity with this kernel module. But that's boring, right? :)

```bash
./install.sh
```

## Uninstallation procedure

To uninstall kernKill and revert all changes made to your system, including re-enabling the `uas` and `usb-storage` drivers, use the following command:

```bash
./rollback.sh
```

## Debugging tips

For debugging, you can monitor kernel messages related to kernKill using:

```bash
sudo dmesg | grep kernKill
```

## Additional information

Note: This module was developed during dem-hack5 and is currently not recommended for real-world use. It has been tested on the x86_64 architecture with the Linux kernel version `6.5.6-76060506-generic`. The module theoretically supports any x86/x86_64 machine with relatively modern linux kernel, but has not been extensively tested in different environments.

Always use caution and don't use it in a production environment.
