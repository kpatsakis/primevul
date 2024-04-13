static uint64_t xhci_oper_read(void *ptr, hwaddr reg, unsigned size)
{
    XHCIState *xhci = ptr;
    uint32_t ret;

    switch (reg) {
    case 0x00: /* USBCMD */
        ret = xhci->usbcmd;
        break;
    case 0x04: /* USBSTS */
        ret = xhci->usbsts;
        break;
    case 0x08: /* PAGESIZE */
        ret = 1; /* 4KiB */
        break;
    case 0x14: /* DNCTRL */
        ret = xhci->dnctrl;
        break;
    case 0x18: /* CRCR low */
        ret = xhci->crcr_low & ~0xe;
        break;
    case 0x1c: /* CRCR high */
        ret = xhci->crcr_high;
        break;
    case 0x30: /* DCBAAP low */
        ret = xhci->dcbaap_low;
        break;
    case 0x34: /* DCBAAP high */
        ret = xhci->dcbaap_high;
        break;
    case 0x38: /* CONFIG */
        ret = xhci->config;
        break;
    default:
        trace_usb_xhci_unimplemented("oper read", reg);
        ret = 0;
    }

    trace_usb_xhci_oper_read(reg, ret);
    return ret;
}
