static void xhci_oper_write(void *ptr, hwaddr reg,
                            uint64_t val, unsigned size)
{
    XHCIState *xhci = ptr;
    DeviceState *d = DEVICE(ptr);

    trace_usb_xhci_oper_write(reg, val);

    switch (reg) {
    case 0x00: /* USBCMD */
        if ((val & USBCMD_RS) && !(xhci->usbcmd & USBCMD_RS)) {
            xhci_run(xhci);
        } else if (!(val & USBCMD_RS) && (xhci->usbcmd & USBCMD_RS)) {
            xhci_stop(xhci);
        }
        if (val & USBCMD_CSS) {
            /* save state */
            xhci->usbsts &= ~USBSTS_SRE;
        }
        if (val & USBCMD_CRS) {
            /* restore state */
            xhci->usbsts |= USBSTS_SRE;
        }
        xhci->usbcmd = val & 0xc0f;
        xhci_mfwrap_update(xhci);
        if (val & USBCMD_HCRST) {
            xhci_reset(d);
        }
        xhci_intx_update(xhci);
        break;

    case 0x04: /* USBSTS */
        /* these bits are write-1-to-clear */
        xhci->usbsts &= ~(val & (USBSTS_HSE|USBSTS_EINT|USBSTS_PCD|USBSTS_SRE));
        xhci_intx_update(xhci);
        break;

    case 0x14: /* DNCTRL */
        xhci->dnctrl = val & 0xffff;
        break;
    case 0x18: /* CRCR low */
        xhci->crcr_low = (val & 0xffffffcf) | (xhci->crcr_low & CRCR_CRR);
        break;
    case 0x1c: /* CRCR high */
        xhci->crcr_high = val;
        if (xhci->crcr_low & (CRCR_CA|CRCR_CS) && (xhci->crcr_low & CRCR_CRR)) {
            XHCIEvent event = {ER_COMMAND_COMPLETE, CC_COMMAND_RING_STOPPED};
            xhci->crcr_low &= ~CRCR_CRR;
            xhci_event(xhci, &event, 0);
            DPRINTF("xhci: command ring stopped (CRCR=%08x)\n", xhci->crcr_low);
        } else {
            dma_addr_t base = xhci_addr64(xhci->crcr_low & ~0x3f, val);
            xhci_ring_init(xhci, &xhci->cmd_ring, base);
        }
        xhci->crcr_low &= ~(CRCR_CA | CRCR_CS);
        break;
    case 0x30: /* DCBAAP low */
        xhci->dcbaap_low = val & 0xffffffc0;
        break;
    case 0x34: /* DCBAAP high */
        xhci->dcbaap_high = val;
        break;
    case 0x38: /* CONFIG */
        xhci->config = val & 0xff;
        break;
    default:
        trace_usb_xhci_unimplemented("oper write", reg);
    }
}
