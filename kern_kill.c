/*******************************************************************************
 *  \file       kern_kill.c
 *
 *  \details    kern_kill USB driver
 *
 *  \author     sigseg5
 *
 *  \Tested on 6.5.6-76060506-generic  Linux kernel
 *
 *******************************************************************************/

#define _GNU_SOURCE

#include <linux/reboot.h>
#include <linux/usb.h>

#include "proc_list.h"

static int usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_device *udev = interface_to_usbdev(interface);
    char serial_number[256];
    if (udev->descriptor.iSerialNumber > 0)
    {
        int ret = usb_string(udev,
                             udev->descriptor.iSerialNumber,
                             serial_number,
                             sizeof(serial_number));
        if (ret > 0)
        {
            printk(KERN_INFO "kernKill: USB device connected (Vendor ID: 0x%04X, Product ID: 0x%04X, Serial Number: %s)\n",
                   le16_to_cpu(udev->descriptor.idVendor),
                   le16_to_cpu(udev->descriptor.idProduct),
                   serial_number);
        }
        else
        {
            printk(KERN_INFO "kernKill: Error retrieving USB device serial number\n");
        }
    }
    else
    {
        printk(KERN_INFO "kernKill: USB device does not have a serial number\n");
    }
    return 0;
}

static void usb_disconnect(struct usb_interface *interface)
{
    struct usb_device *udev = interface_to_usbdev(interface);
    printk(KERN_INFO "kernKill: USB device disconnected (Vendor ID: 0x%04X, Product ID: 0x%04X)\n",
           le16_to_cpu(udev->descriptor.idVendor),
           le16_to_cpu(udev->descriptor.idProduct));

    if (unlikely(udev->descriptor.idVendor == USB_VENDOR_ID && udev->descriptor.idProduct))
    {
        printk(KERN_INFO "kernKill: USB device signature matches!");

#ifdef MODE_REBOOT
        emergency_restart();

        // Thus, this code is only compatible with x86_64
        // The __NR_REBOOT system call is used
        // But it doesn't really work as expected :)

        // asm volatile(
        //     "mov     $0xa9,       %al\n\t"
        //     "mov     $0xfee1dead, %edi\n\t"
        //     "mov     $0x28121969, %esi\n\t"
        //     "mov     $0x4321fedc, %edx\n\t"
        //     "syscall\n\t");

        // This for some reason disables bluetooth and the ability to correctly perform a kekw shutdown
        // STP instruction (0xDB):
        // STP stops the clock input of the 65C02, effectively shutting down the 65C02 until a hardware reset occurs

        // asm volatile("mov $0xDB, %ax\n\t"
        //              "ltr %ax");
#endif

#ifdef MODE_KILL
        struct task_struct *task;
        for_each_process(task)
        {
            for (int i = 0; i < PROC_COUNT; i++)
            {
                if (unlikely(strcmp(task->comm, KILL_PROC[i]) == 0))
                {
                    printk(KERN_INFO "kernKill: Matched proc: %s, pid = %d\n", task->comm, task->pid);
                    struct siginfo info;
                    memset(&info, 0, sizeof(struct siginfo));
                    info.si_signo = SIGKILL;
                    info.si_code = SI_QUEUE;
                    info.si_int = 1;
                    if (task != NULL)
                    {
                        printk(KERN_INFO "kernKill: Sending signal to app\n");
                        if (send_sig_info(SIGKILL, (struct kernel_siginfo *)&info, task) < 0)
                        {
                            printk(KERN_WARNING "kernKill: Unable to send signal\n");
                        }
                    }
                    break;
                }
            }
        }
#endif
    }
}

static struct usb_device_id usb_table[] = {
    {USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID)},
    {}
};

MODULE_DEVICE_TABLE(usb, usb_table);

static struct usb_driver usb_driver = {
    .name = "kern_kill",
    .probe = usb_probe,
    .disconnect = usb_disconnect,
    .id_table = usb_table,
};

static int __init usb_notify_init(void)
{
    int result = usb_register(&usb_driver);
    if (result)
        printk(KERN_ERR "kernKill: USB driver registration failed: %d\n", result);
    return result;
}

static void __exit usb_notify_exit(void)
{
    usb_deregister(&usb_driver);
}

module_init(usb_notify_init);
module_exit(usb_notify_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sigseg5");
MODULE_DESCRIPTION("kern_kill");
MODULE_VERSION("0.1");
