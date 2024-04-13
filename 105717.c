static void usb_net_realize(USBDevice *dev, Error **errrp)
{
    USBNetState *s = USB_NET(dev);

    usb_desc_create_serial(dev);
    usb_desc_init(dev);

    s->rndis_state = RNDIS_UNINITIALIZED;
    QTAILQ_INIT(&s->rndis_resp);

    s->medium = 0;	/* NDIS_MEDIUM_802_3 */
    s->speed = 1000000; /* 100MBps, in 100Bps units */
    s->media_state = 0;	/* NDIS_MEDIA_STATE_CONNECTED */;
    s->filter = 0;
    s->vendorid = 0x1234;
    s->intr = usb_ep_get(dev, USB_TOKEN_IN, 1);

    qemu_macaddr_default_if_unset(&s->conf.macaddr);
    s->nic = qemu_new_nic(&net_usbnet_info, &s->conf,
                          object_get_typename(OBJECT(s)), s->dev.qdev.id, s);
    qemu_format_nic_info_str(qemu_get_queue(s->nic), s->conf.macaddr.a);
    snprintf(s->usbstring_mac, sizeof(s->usbstring_mac),
             "%02x%02x%02x%02x%02x%02x",
             0x40,
             s->conf.macaddr.a[1],
             s->conf.macaddr.a[2],
             s->conf.macaddr.a[3],
             s->conf.macaddr.a[4],
             s->conf.macaddr.a[5]);
    usb_desc_set_string(dev, STRING_ETHADDR, s->usbstring_mac);
}
