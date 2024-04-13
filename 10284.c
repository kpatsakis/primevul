static void zynq_slcr_ps_clk_callback(void *opaque)
{
    ZynqSLCRState *s = (ZynqSLCRState *) opaque;
    zynq_slcr_compute_clocks(s);
    zynq_slcr_propagate_clocks(s);
}