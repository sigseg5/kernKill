#define PRINT_USB_INTERFACE_DESCRIPTOR(i)                                      \
  {                                                                            \
    pr_info("USB_INTERFACE_DESCRIPTOR:\n");                                    \
    pr_info("-----------------------------\n");                                \
    pr_info("bLength: 0x%x\n", i.bLength);                                     \
    pr_info("bDescriptorType: 0x%x\n", i.bDescriptorType);                     \
    pr_info("bInterfaceNumber: 0x%x\n", i.bInterfaceNumber);                   \
    pr_info("bAlternateSetting: 0x%x\n", i.bAlternateSetting);                 \
    pr_info("bNumEndpoints: 0x%x\n", i.bNumEndpoints);                         \
    pr_info("bInterfaceClass: 0x%x\n", i.bInterfaceClass);                     \
    pr_info("bInterfaceSubClass: 0x%x\n", i.bInterfaceSubClass);               \
    pr_info("bInterfaceProtocol: 0x%x\n", i.bInterfaceProtocol);               \
    pr_info("iInterface: 0x%x\n", i.iInterface);                               \
    pr_info("\n");                                                             \
  }
#define PRINT_USB_ENDPOINT_DESCRIPTOR(e)                                       \
  {                                                                            \
    pr_info("USB_ENDPOINT_DESCRIPTOR:\n");                                     \
    pr_info("------------------------\n");                                     \
    pr_info("bLength: 0x%x\n", e.bLength);                                     \
    pr_info("bDescriptorType: 0x%x\n", e.bDescriptorType);                     \
    pr_info("bEndPointAddress: 0x%x\n", e.bEndpointAddress);                   \
    pr_info("bmAttributes: 0x%x\n", e.bmAttributes);                           \
    pr_info("wMaxPacketSize: 0x%x\n", e.wMaxPacketSize);                       \
    pr_info("bInterval: 0x%x\n", e.bInterval);                                 \
    pr_info("\n");                                                             \
  }
