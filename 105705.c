static void usb_net_handle_datain(USBNetState *s, USBPacket *p)
{
    int len;

    if (s->in_ptr > s->in_len) {
        usb_net_reset_in_buf(s);
        p->status = USB_RET_NAK;
        return;
    }
    if (!s->in_len) {
        p->status = USB_RET_NAK;
        return;
    }
    len = s->in_len - s->in_ptr;
    if (len > p->iov.size) {
        len = p->iov.size;
    }
    usb_packet_copy(p, &s->in_buf[s->in_ptr], len);
    s->in_ptr += len;
    if (s->in_ptr >= s->in_len &&
                    (is_rndis(s) || (s->in_len & (64 - 1)) || !len)) {
        /* no short packet necessary */
        usb_net_reset_in_buf(s);
    }

#ifdef TRAFFIC_DEBUG
    fprintf(stderr, "usbnet: data in len %zu return %d", p->iov.size, len);
    iov_hexdump(p->iov.iov, p->iov.niov, stderr, "usbnet", len);
#endif
}
