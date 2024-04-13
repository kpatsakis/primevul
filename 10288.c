static void zynq_slcr_propagate_clocks(ZynqSLCRState *s)
{
    clock_propagate(s->uart0_ref_clk);
    clock_propagate(s->uart1_ref_clk);
}