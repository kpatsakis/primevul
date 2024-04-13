static void zynq_slcr_reset_exit(Object *obj)
{
    ZynqSLCRState *s = ZYNQ_SLCR(obj);

    /* will compute output clocks according to ps_clk and registers */
    zynq_slcr_compute_clocks(s);
    zynq_slcr_propagate_clocks(s);
}