static void xhci_doorbell_write(void *ptr, hwaddr reg,
                                uint64_t val, unsigned size)
{
    XHCIState *xhci = ptr;
    unsigned int epid, streamid;

    trace_usb_xhci_doorbell_write(reg, val);

    if (!xhci_running(xhci)) {
        DPRINTF("xhci: wrote doorbell while xHC stopped or paused\n");
        return;
    }

    reg >>= 2;

    if (reg == 0) {
        if (val == 0) {
            xhci_process_commands(xhci);
        } else {
            DPRINTF("xhci: bad doorbell 0 write: 0x%x\n",
                    (uint32_t)val);
        }
    } else {
        epid = val & 0xff;
        streamid = (val >> 16) & 0xffff;
        if (reg > xhci->numslots) {
            DPRINTF("xhci: bad doorbell %d\n", (int)reg);
        } else if (epid > 31) {
            DPRINTF("xhci: bad doorbell %d write: 0x%x\n",
                    (int)reg, (uint32_t)val);
        } else {
            xhci_kick_ep(xhci, reg, epid, streamid);
        }
    }
}
