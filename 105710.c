static void usb_net_handle_data(USBDevice *dev, USBPacket *p)
{
    USBNetState *s = (USBNetState *) dev;

    switch(p->pid) {
    case USB_TOKEN_IN:
        switch (p->ep->nr) {
        case 1:
            usb_net_handle_statusin(s, p);
            break;

        case 2:
            usb_net_handle_datain(s, p);
            break;

        default:
            goto fail;
        }
        break;

    case USB_TOKEN_OUT:
        switch (p->ep->nr) {
        case 2:
            usb_net_handle_dataout(s, p);
            break;

        default:
            goto fail;
        }
        break;

    default:
    fail:
        p->status = USB_RET_STALL;
        break;
    }

    if (p->status == USB_RET_STALL) {
        fprintf(stderr, "usbnet: failed data transaction: "
                        "pid 0x%x ep 0x%x len 0x%zx\n",
                        p->pid, p->ep->nr, p->iov.size);
    }
}
