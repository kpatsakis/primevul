static void usb_net_handle_statusin(USBNetState *s, USBPacket *p)
{
    le32 buf[2];

    if (p->iov.size < 8) {
        p->status = USB_RET_STALL;
        return;
    }

    buf[0] = cpu_to_le32(1);
    buf[1] = cpu_to_le32(0);
    usb_packet_copy(p, buf, 8);
    if (!s->rndis_resp.tqh_first) {
        p->status = USB_RET_NAK;
    }

#ifdef TRAFFIC_DEBUG
    fprintf(stderr, "usbnet: interrupt poll len %zu return %d",
            p->iov.size, p->status);
    iov_hexdump(p->iov.iov, p->iov.niov, stderr, "usbnet", p->status);
#endif
}
