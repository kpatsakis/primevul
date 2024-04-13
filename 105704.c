static void usb_net_handle_control(USBDevice *dev, USBPacket *p,
               int request, int value, int index, int length, uint8_t *data)
{
    USBNetState *s = (USBNetState *) dev;
    int ret;

    ret = usb_desc_handle_control(dev, p, request, value, index, length, data);
    if (ret >= 0) {
        return;
    }

    switch(request) {
    case ClassInterfaceOutRequest | USB_CDC_SEND_ENCAPSULATED_COMMAND:
        if (!is_rndis(s) || value || index != 0) {
            goto fail;
        }
#ifdef TRAFFIC_DEBUG
        {
            unsigned int i;
            fprintf(stderr, "SEND_ENCAPSULATED_COMMAND:");
            for (i = 0; i < length; i++) {
                if (!(i & 15))
                    fprintf(stderr, "\n%04x:", i);
                fprintf(stderr, " %02x", data[i]);
            }
            fprintf(stderr, "\n\n");
        }
#endif
        ret = rndis_parse(s, data, length);
        if (ret < 0) {
            p->status = ret;
        }
        break;

    case ClassInterfaceRequest | USB_CDC_GET_ENCAPSULATED_RESPONSE:
        if (!is_rndis(s) || value || index != 0) {
            goto fail;
        }
        p->actual_length = rndis_get_response(s, data);
        if (p->actual_length == 0) {
            data[0] = 0;
            p->actual_length = 1;
        }
#ifdef TRAFFIC_DEBUG
        {
            unsigned int i;
            fprintf(stderr, "GET_ENCAPSULATED_RESPONSE:");
            for (i = 0; i < p->actual_length; i++) {
                if (!(i & 15))
                    fprintf(stderr, "\n%04x:", i);
                fprintf(stderr, " %02x", data[i]);
            }
            fprintf(stderr, "\n\n");
        }
#endif
        break;

    default:
    fail:
        fprintf(stderr, "usbnet: failed control transaction: "
                        "request 0x%x value 0x%x index 0x%x length 0x%x\n",
                        request, value, index, length);
        p->status = USB_RET_STALL;
        break;
    }
}
