static uint64_t xhci_cap_read(void *ptr, hwaddr reg, unsigned size)
{
    XHCIState *xhci = ptr;
    uint32_t ret;

    switch (reg) {
    case 0x00: /* HCIVERSION, CAPLENGTH */
        ret = 0x01000000 | LEN_CAP;
        break;
    case 0x04: /* HCSPARAMS 1 */
        ret = ((xhci->numports_2+xhci->numports_3)<<24)
            | (xhci->numintrs<<8) | xhci->numslots;
        break;
    case 0x08: /* HCSPARAMS 2 */
        ret = 0x0000000f;
        break;
    case 0x0c: /* HCSPARAMS 3 */
        ret = 0x00000000;
        break;
    case 0x10: /* HCCPARAMS */
        if (sizeof(dma_addr_t) == 4) {
            ret = 0x00080000 | (xhci->max_pstreams_mask << 12);
        } else {
            ret = 0x00080001 | (xhci->max_pstreams_mask << 12);
        }
        break;
    case 0x14: /* DBOFF */
        ret = OFF_DOORBELL;
        break;
    case 0x18: /* RTSOFF */
        ret = OFF_RUNTIME;
        break;

    /* extended capabilities */
    case 0x20: /* Supported Protocol:00 */
        ret = 0x02000402; /* USB 2.0 */
        break;
    case 0x24: /* Supported Protocol:04 */
        ret = 0x20425355; /* "USB " */
        break;
    case 0x28: /* Supported Protocol:08 */
        if (xhci_get_flag(xhci, XHCI_FLAG_SS_FIRST)) {
            ret = (xhci->numports_2<<8) | (xhci->numports_3+1);
        } else {
            ret = (xhci->numports_2<<8) | 1;
        }
        break;
    case 0x2c: /* Supported Protocol:0c */
        ret = 0x00000000; /* reserved */
        break;
    case 0x30: /* Supported Protocol:00 */
        ret = 0x03000002; /* USB 3.0 */
        break;
    case 0x34: /* Supported Protocol:04 */
        ret = 0x20425355; /* "USB " */
        break;
    case 0x38: /* Supported Protocol:08 */
        if (xhci_get_flag(xhci, XHCI_FLAG_SS_FIRST)) {
            ret = (xhci->numports_3<<8) | 1;
        } else {
            ret = (xhci->numports_3<<8) | (xhci->numports_2+1);
        }
        break;
    case 0x3c: /* Supported Protocol:0c */
        ret = 0x00000000; /* reserved */
        break;
    default:
        trace_usb_xhci_unimplemented("cap read", reg);
        ret = 0;
    }

    trace_usb_xhci_cap_read(reg, ret);
    return ret;
}
