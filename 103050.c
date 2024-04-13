static int lsi_queue_req(LSIState *s, SCSIRequest *req, uint32_t len)
{
    lsi_request *p = req->hba_private;

    if (p->pending) {
        trace_lsi_queue_req_error(p);
    }
    p->pending = len;
    /* Reselect if waiting for it, or if reselection triggers an IRQ
       and the bus is free.
       Since no interrupt stacking is implemented in the emulation, it
       is also required that there are no pending interrupts waiting
       for service from the device driver. */
    if (s->waiting == LSI_WAIT_RESELECT ||
        (lsi_irq_on_rsl(s) && !(s->scntl1 & LSI_SCNTL1_CON) &&
         !(s->istat0 & (LSI_ISTAT0_SIP | LSI_ISTAT0_DIP)))) {
        /* Reselect device.  */
        lsi_reselect(s, p);
        return 0;
    } else {
        trace_lsi_queue_req(p->tag);
        p->pending = len;
        return 1;
    }
}
