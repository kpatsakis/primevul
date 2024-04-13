static void usb_net_class_initfn(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    USBDeviceClass *uc = USB_DEVICE_CLASS(klass);

    uc->realize        = usb_net_realize;
    uc->product_desc   = "QEMU USB Network Interface";
    uc->usb_desc       = &desc_net;
    uc->handle_reset   = usb_net_handle_reset;
    uc->handle_control = usb_net_handle_control;
    uc->handle_data    = usb_net_handle_data;
    uc->handle_destroy = usb_net_handle_destroy;
    set_bit(DEVICE_CATEGORY_NETWORK, dc->categories);
    dc->fw_name = "network";
    dc->vmsd = &vmstate_usb_net;
    dc->props = net_properties;
}
