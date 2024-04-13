static void lsi_update_irq(LSIState *s)
{
    int level;
    static int last_level;

    /* It's unclear whether the DIP/SIP bits should be cleared when the
       Interrupt Status Registers are cleared or when istat0 is read.
       We currently do the formwer, which seems to work.  */
    level = 0;
    if (s->dstat) {
        if (s->dstat & s->dien)
            level = 1;
        s->istat0 |= LSI_ISTAT0_DIP;
    } else {
        s->istat0 &= ~LSI_ISTAT0_DIP;
    }

    if (s->sist0 || s->sist1) {
        if ((s->sist0 & s->sien0) || (s->sist1 & s->sien1))
            level = 1;
        s->istat0 |= LSI_ISTAT0_SIP;
    } else {
        s->istat0 &= ~LSI_ISTAT0_SIP;
    }
    if (s->istat0 & LSI_ISTAT0_INTF)
        level = 1;

    if (level != last_level) {
        trace_lsi_update_irq(level, s->dstat, s->sist1, s->sist0);
        last_level = level;
    }
    lsi_set_irq(s, level);

    if (!s->current && !level && lsi_irq_on_rsl(s) && !(s->scntl1 & LSI_SCNTL1_CON)) {
        lsi_request *p;

        trace_lsi_update_irq_disconnected();
        p = get_pending_req(s);
        if (p) {
            lsi_reselect(s, p);
        }
    }
}
