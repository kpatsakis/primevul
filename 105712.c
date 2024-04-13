static void usb_net_handle_destroy(USBDevice *dev)
{
    USBNetState *s = (USBNetState *) dev;

    /* TODO: remove the nd_table[] entry */
    rndis_clear_responsequeue(s);
    qemu_del_nic(s->nic);
}
