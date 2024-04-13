static int xhci_complete_packet(XHCITransfer *xfer)
{
    if (xfer->packet.status == USB_RET_ASYNC) {
        trace_usb_xhci_xfer_async(xfer);
        xfer->running_async = 1;
        xfer->running_retry = 0;
        xfer->complete = 0;
        return 0;
    } else if (xfer->packet.status == USB_RET_NAK) {
        trace_usb_xhci_xfer_nak(xfer);
        xfer->running_async = 0;
        xfer->running_retry = 1;
        xfer->complete = 0;
        return 0;
    } else {
        xfer->running_async = 0;
        xfer->running_retry = 0;
        xfer->complete = 1;
        xhci_xfer_unmap(xfer);
    }

    if (xfer->packet.status == USB_RET_SUCCESS) {
        trace_usb_xhci_xfer_success(xfer, xfer->packet.actual_length);
        xfer->status = CC_SUCCESS;
        xhci_xfer_report(xfer);
        return 0;
    }

    /* error */
    trace_usb_xhci_xfer_error(xfer, xfer->packet.status);
    switch (xfer->packet.status) {
    case USB_RET_NODEV:
    case USB_RET_IOERROR:
        xfer->status = CC_USB_TRANSACTION_ERROR;
        xhci_xfer_report(xfer);
        xhci_stall_ep(xfer);
        break;
    case USB_RET_STALL:
        xfer->status = CC_STALL_ERROR;
        xhci_xfer_report(xfer);
        xhci_stall_ep(xfer);
        break;
    case USB_RET_BABBLE:
        xfer->status = CC_BABBLE_DETECTED;
        xhci_xfer_report(xfer);
        xhci_stall_ep(xfer);
        break;
    default:
        DPRINTF("%s: FIXME: status = %d\n", __func__,
                xfer->packet.status);
        FIXME("unhandled USB_RET_*");
    }
    return 0;
}
