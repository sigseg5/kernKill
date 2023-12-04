/*******************************************************************************
 *  \file       kernKill.c
 *
 *  \details    kern_kill USB driver
 *
 *  \author     sigseg5
 *
 *  \Tested with 6.5.6-76060506-generic
 *
 *******************************************************************************/

#define _GNU_SOURCE

#include <linux/reboot.h>
#include <linux/usb.h>

// #include <asm/io.h>

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
            // Error retrieving the serial number
            printk(KERN_INFO "kernKill: Error retrieving USB device serial number\n");
        }
    }
    else
    {
        // Device does not have a serial number
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
        printk(KERN_INFO "kernKill: kill switch USB device is matched!");

#ifdef MODE_REBOOT
        // That actually works
        emergency_restart();

        // asm("cli\n"
        //     "hlt");

        // So, this code is only x86_64 compatible
        // Exploitation of the __NR_REBOOT syscall
        // But that doesn't actually work as expected :)

        // asm volatile(
        //     "mov     $0xa9,       %al\n\t"
        //     "mov     $0xfee1dead, %edi\n\t"
        //     "mov     $0x28121969, %esi\n\t"
        //     "mov     $0x4321fedc, %edx\n\t"
        //     "syscall\n\t");

        // That disable bluetooth and ability to perform properly shutdown kekw
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
                            printk(KERN_WARNING "kernKill:Unable to send signal\n");
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
    {} // Terminating entry
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
