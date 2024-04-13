static void lsi_script_dma_interrupt(LSIState *s, int stat)
{
    trace_lsi_script_dma_interrupt(stat, s->dstat);
    s->dstat |= stat;
    lsi_update_irq(s);
    lsi_stop_script(s);
}
