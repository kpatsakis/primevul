static void usbnet_cleanup(NetClientState *nc)
{
    USBNetState *s = qemu_get_nic_opaque(nc);

    s->nic = NULL;
}
