static void lsi_bad_phase(LSIState *s, int out, int new_phase)
{
    /* Trigger a phase mismatch.  */
    if (s->ccntl0 & LSI_CCNTL0_ENPMJ) {
        if ((s->ccntl0 & LSI_CCNTL0_PMJCTL)) {
            s->dsp = out ? s->pmjad1 : s->pmjad2;
        } else {
            s->dsp = (s->scntl2 & LSI_SCNTL2_WSR ? s->pmjad2 : s->pmjad1);
        }
        trace_lsi_bad_phase_jump(s->dsp);
    } else {
        trace_lsi_bad_phase_interrupt();
        lsi_script_scsi_interrupt(s, LSI_SIST0_MA, 0);
        lsi_stop_script(s);
    }
    lsi_set_phase(s, new_phase);
}
