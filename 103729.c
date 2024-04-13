static int xhci_ep_nuke_one_xfer(XHCITransfer *t, TRBCCode report)
{
    int killed = 0;

    if (report && (t->running_async || t->running_retry)) {
        t->status = report;
        xhci_xfer_report(t);
    }

    if (t->running_async) {
        usb_cancel_packet(&t->packet);
        t->running_async = 0;
        killed = 1;
    }
    if (t->running_retry) {
        if (t->epctx) {
            t->epctx->retry = NULL;
            timer_del(t->epctx->kick_timer);
        }
        t->running_retry = 0;
        killed = 1;
    }
    g_free(t->trbs);

    t->trbs = NULL;
    t->trb_count = 0;

    return killed;
}
