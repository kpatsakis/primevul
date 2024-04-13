static void lsi_transfer_data(SCSIRequest *req, uint32_t len)
{
    LSIState *s = LSI53C895A(req->bus->qbus.parent);
    int out;

    assert(req->hba_private);
    if (s->waiting == LSI_WAIT_RESELECT || req->hba_private != s->current ||
        (lsi_irq_on_rsl(s) && !(s->scntl1 & LSI_SCNTL1_CON))) {
        if (lsi_queue_req(s, req, len)) {
            return;
        }
    }

    out = (s->sstat1 & PHASE_MASK) == PHASE_DO;

    /* host adapter (re)connected */
    trace_lsi_transfer_data(req->tag, len);
    s->current->dma_len = len;
    s->command_complete = 1;
    if (s->waiting) {
        if (s->waiting == LSI_WAIT_RESELECT || s->dbc == 0) {
            lsi_resume_script(s);
        } else {
            lsi_do_dma(s, out);
        }
    }
}
