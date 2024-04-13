static void zynq_slcr_compute_clocks(ZynqSLCRState *s)
{
    uint64_t ps_clk = clock_get(s->ps_clk);

    /* consider outputs clocks are disabled while in reset */
    if (device_is_in_reset(DEVICE(s))) {
        ps_clk = 0;
    }

    uint64_t io_pll = zynq_slcr_compute_pll(ps_clk, s->regs[R_IO_PLL_CTRL]);
    uint64_t arm_pll = zynq_slcr_compute_pll(ps_clk, s->regs[R_ARM_PLL_CTRL]);
    uint64_t ddr_pll = zynq_slcr_compute_pll(ps_clk, s->regs[R_DDR_PLL_CTRL]);

    uint64_t uart_mux[4] = {io_pll, io_pll, arm_pll, ddr_pll};

    /* compute uartX reference clocks */
    clock_set(s->uart0_ref_clk,
              ZYNQ_COMPUTE_CLK(s, uart_mux, R_UART_CLK_CTRL, CLKACT0));
    clock_set(s->uart1_ref_clk,
              ZYNQ_COMPUTE_CLK(s, uart_mux, R_UART_CLK_CTRL, CLKACT1));
}