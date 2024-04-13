static void zynq_slcr_reset_hold(Object *obj)
{
    ZynqSLCRState *s = ZYNQ_SLCR(obj);

    /* will disable all output clocks */
    zynq_slcr_compute_clocks(s);
    zynq_slcr_propagate_clocks(s);
}