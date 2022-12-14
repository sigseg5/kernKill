/*******************************************************************************
 *  \file       kernKill.c
 *
 *  \details    kernKill USB driver
 *
 *  \author     sigseg5
 *
 *  \Tested with kernel 5.15.0-48-generic
 *
 *******************************************************************************/
#define _GNU_SOURCE

#include "macros.h"
#include "proc.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/sched.h>
#include <linux/usb.h>

// get this values by dmesg
#define USB_VENDOR_ID (0x0951)  // USB device's vendor ID
#define USB_PRODUCT_ID (0x172b) // USB device's product ID

int kill_pid_info(int sig, struct kernel_siginfo *info, struct pid *pid) {
  int error = -ESRCH;
  struct task_struct *p;

  for (;;) {
    rcu_read_lock();
    p = pid_task(pid, PIDTYPE_PID);
    // TODO: catch err
    // if (p)
    // error = group_send_sig_info(sig, info, p, PIDTYPE_TGID);
    rcu_read_unlock();
    if (likely(!p || error != -ESRCH))
      return error;
  }
}

static int kill_proc_info(int sig, struct kernel_siginfo *info, pid_t pid) {
  int error;
  rcu_read_lock();
  error = kill_pid_info(sig, info, find_vpid(pid));
  rcu_read_unlock();
  return error;
}
static int etx_usb_probe(struct usb_interface *interface,
                         const struct usb_device_id *id) {
  unsigned int i;
  unsigned int endpoints_count;
  struct usb_host_interface *iface_desc = interface->cur_altsetting;

  dev_info(&interface->dev,
           "USB Driver Probed: Vendor ID : 0x%02x,\t"
           "Product ID : 0x%02x\n",
           id->idVendor, id->idProduct);

  if (unlikely(id->idVendor == USB_VENDOR_ID &&
               id->idProduct == USB_PRODUCT_ID)) {
    // kernel_power_off();
    struct task_struct *task;
    for_each_process(task) {
      printk(KERN_INFO "Process %i is named %s\n", task->pid, task->comm);
      for (int i = 0; i < PROC_COUNT; i++) {
        if (unlikely(task->comm == KILL_PROC[PROC_COUNT])) {
          kill_proc_info(SIGKILL, SEND_SIG_PRIV, (pid_t)task->pid);
          return 0;
        }
      }
    }
    return 0;
  }

  endpoints_count = iface_desc->desc.bNumEndpoints;

  for (i = 0; i < endpoints_count; i++) {
    // PRINT_USB_ENDPOINT_DESCRIPTOR(iface_desc->endpoint[i].desc);
  }
  return 0;
}

static void etx_usb_disconnect(struct usb_interface *interface) {
  dev_info(&interface->dev, "USB Driver Disconnected\n");
}
// usb_device_id provides a list of different types of USB devices that the
// driver supports
const struct usb_device_id etx_usb_table[] = {
    {USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID)}, {}};

// This enable the linux hotplug system to load the driver automatically when
// the device is plugged in
MODULE_DEVICE_TABLE(usb, etx_usb_table);
// The structure needs to do is register with the linux subsystem
static struct usb_driver etx_usb_driver = {
    .name = "kernKill USB Driver",
    .probe = etx_usb_probe,
    .disconnect = etx_usb_disconnect,
    .id_table = etx_usb_table,
};

module_usb_driver(etx_usb_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sigseg5 <sigseg5@protonmail.com>");
MODULE_DESCRIPTION("kernKill");
MODULE_VERSION("0.1");
