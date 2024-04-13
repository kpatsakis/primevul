static void usb_net_register_types(void)
{
    type_register_static(&net_info);
    usb_legacy_register(TYPE_USB_NET, "net", usb_net_init);
}
