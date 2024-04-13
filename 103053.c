static void lsi_reselect(LSIState *s, lsi_request *p)
{
    int id;

    assert(s->current == NULL);
    QTAILQ_REMOVE(&s->queue, p, next);
    s->current = p;

    id = (p->tag >> 8) & 0xf;
    s->ssid = id | 0x80;
    /* LSI53C700 Family Compatibility, see LSI53C895A 4-73 */
    if (!(s->dcntl & LSI_DCNTL_COM)) {
        s->sfbr = 1 << (id & 0x7);
    }
    trace_lsi_reselect(id);
    s->scntl1 |= LSI_SCNTL1_CON;
    lsi_set_phase(s, PHASE_MI);
    s->msg_action = p->out ? LSI_MSG_ACTION_DOUT : LSI_MSG_ACTION_DIN;
    s->current->dma_len = p->pending;
    lsi_add_msg_byte(s, 0x80);
    if (s->current->tag & LSI_TAG_VALID) {
        lsi_add_msg_byte(s, 0x20);
        lsi_add_msg_byte(s, p->tag & 0xff);
    }

    if (lsi_irq_on_rsl(s)) {
        lsi_script_scsi_interrupt(s, LSI_SIST0_RSL, 0);
    }
}
