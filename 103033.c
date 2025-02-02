static void lsi_command_complete(SCSIRequest *req, uint32_t status, size_t resid)
{
    LSIState *s = LSI53C895A(req->bus->qbus.parent);
    int out;

    out = (s->sstat1 & PHASE_MASK) == PHASE_DO;
    trace_lsi_command_complete(status);
    s->status = status;
    s->command_complete = 2;
    if (s->waiting && s->dbc != 0) {
        /* Raise phase mismatch for short transfers.  */
        lsi_bad_phase(s, out, PHASE_ST);
    } else {
        lsi_set_phase(s, PHASE_ST);
    }

    if (req->hba_private == s->current) {
        req->hba_private = NULL;
        lsi_request_free(s, s->current);
        scsi_req_unref(req);
    }
    lsi_resume_script(s);
}
