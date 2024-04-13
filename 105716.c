static void usb_net_instance_init(Object *obj)
{
    USBDevice *dev = USB_DEVICE(obj);
    USBNetState *s = USB_NET(dev);

    device_add_bootindex_property(obj, &s->conf.bootindex,
                                  "bootindex", "/ethernet-phy@0",
                                  &dev->qdev, NULL);
}
