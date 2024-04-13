static void lsi_bad_selection(LSIState *s, uint32_t id)
{
    trace_lsi_bad_selection(id);
    lsi_script_scsi_interrupt(s, 0, LSI_SIST1_STO);
    lsi_disconnect(s);
}
