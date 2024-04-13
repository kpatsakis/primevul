static void lsi_wait_reselect(LSIState *s)
{
    lsi_request *p;

    trace_lsi_wait_reselect();

    if (s->current) {
        return;
    }
    p = get_pending_req(s);
    if (p) {
        lsi_reselect(s, p);
    }
    if (s->current == NULL) {
        s->waiting = LSI_WAIT_RESELECT;
    }
}
