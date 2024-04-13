static void zynq_slcr_reset_init(Object *obj, ResetType type)
{
    ZynqSLCRState *s = ZYNQ_SLCR(obj);
    int i;

    DB_PRINT("RESET\n");

    s->regs[R_LOCKSTA] = 1;
    /* 0x100 - 0x11C */
    s->regs[R_ARM_PLL_CTRL]   = 0x0001A008;
    s->regs[R_DDR_PLL_CTRL]   = 0x0001A008;
    s->regs[R_IO_PLL_CTRL]    = 0x0001A008;
    s->regs[R_PLL_STATUS]     = 0x0000003F;
    s->regs[R_ARM_PLL_CFG]    = 0x00014000;
    s->regs[R_DDR_PLL_CFG]    = 0x00014000;
    s->regs[R_IO_PLL_CFG]     = 0x00014000;

    /* 0x120 - 0x16C */
    s->regs[R_ARM_CLK_CTRL]   = 0x1F000400;
    s->regs[R_DDR_CLK_CTRL]   = 0x18400003;
    s->regs[R_DCI_CLK_CTRL]   = 0x01E03201;
    s->regs[R_APER_CLK_CTRL]  = 0x01FFCCCD;
    s->regs[R_USB0_CLK_CTRL]  = s->regs[R_USB1_CLK_CTRL]  = 0x00101941;
    s->regs[R_GEM0_RCLK_CTRL] = s->regs[R_GEM1_RCLK_CTRL] = 0x00000001;
    s->regs[R_GEM0_CLK_CTRL]  = s->regs[R_GEM1_CLK_CTRL]  = 0x00003C01;
    s->regs[R_SMC_CLK_CTRL]   = 0x00003C01;
    s->regs[R_LQSPI_CLK_CTRL] = 0x00002821;
    s->regs[R_SDIO_CLK_CTRL]  = 0x00001E03;
    s->regs[R_UART_CLK_CTRL]  = 0x00003F03;
    s->regs[R_SPI_CLK_CTRL]   = 0x00003F03;
    s->regs[R_CAN_CLK_CTRL]   = 0x00501903;
    s->regs[R_DBG_CLK_CTRL]   = 0x00000F03;
    s->regs[R_PCAP_CLK_CTRL]  = 0x00000F01;

    /* 0x170 - 0x1AC */
    s->regs[R_FPGA0_CLK_CTRL] = s->regs[R_FPGA1_CLK_CTRL]
                              = s->regs[R_FPGA2_CLK_CTRL]
                              = s->regs[R_FPGA3_CLK_CTRL] = 0x00101800;
    s->regs[R_FPGA0_THR_STA] = s->regs[R_FPGA1_THR_STA]
                             = s->regs[R_FPGA2_THR_STA]
                             = s->regs[R_FPGA3_THR_STA] = 0x00010000;

    /* 0x1B0 - 0x1D8 */
    s->regs[R_BANDGAP_TRIP]   = 0x0000001F;
    s->regs[R_PLL_PREDIVISOR] = 0x00000001;
    s->regs[R_CLK_621_TRUE]   = 0x00000001;

    /* 0x200 - 0x25C */
    s->regs[R_FPGA_RST_CTRL]  = 0x01F33F0F;
    s->regs[R_RST_REASON]     = 0x00000040;

    s->regs[R_BOOT_MODE]      = 0x00000001;

    /* 0x700 - 0x7D4 */
    for (i = 0; i < 54; i++) {
        s->regs[R_MIO + i] = 0x00001601;
    }
    for (i = 2; i <= 8; i++) {
        s->regs[R_MIO + i] = 0x00000601;
    }

    s->regs[R_MIO_MST_TRI0] = s->regs[R_MIO_MST_TRI1] = 0xFFFFFFFF;

    s->regs[R_CPU_RAM + 0] = s->regs[R_CPU_RAM + 1] = s->regs[R_CPU_RAM + 3]
                           = s->regs[R_CPU_RAM + 4] = s->regs[R_CPU_RAM + 7]
                           = 0x00010101;
    s->regs[R_CPU_RAM + 2] = s->regs[R_CPU_RAM + 5] = 0x01010101;
    s->regs[R_CPU_RAM + 6] = 0x00000001;

    s->regs[R_IOU + 0] = s->regs[R_IOU + 1] = s->regs[R_IOU + 2]
                       = s->regs[R_IOU + 3] = 0x09090909;
    s->regs[R_IOU + 4] = s->regs[R_IOU + 5] = 0x00090909;
    s->regs[R_IOU + 6] = 0x00000909;

    s->regs[R_DMAC_RAM] = 0x00000009;

    s->regs[R_AFI0 + 0] = s->regs[R_AFI0 + 1] = 0x09090909;
    s->regs[R_AFI1 + 0] = s->regs[R_AFI1 + 1] = 0x09090909;
    s->regs[R_AFI2 + 0] = s->regs[R_AFI2 + 1] = 0x09090909;
    s->regs[R_AFI3 + 0] = s->regs[R_AFI3 + 1] = 0x09090909;
    s->regs[R_AFI0 + 2] = s->regs[R_AFI1 + 2] = s->regs[R_AFI2 + 2]
                        = s->regs[R_AFI3 + 2] = 0x00000909;

    s->regs[R_OCM + 0] = 0x01010101;
    s->regs[R_OCM + 1] = s->regs[R_OCM + 2] = 0x09090909;

    s->regs[R_DEVCI_RAM] = 0x00000909;
    s->regs[R_CSG_RAM]   = 0x00000001;

    s->regs[R_DDRIOB + 0] = s->regs[R_DDRIOB + 1] = s->regs[R_DDRIOB + 2]
                          = s->regs[R_DDRIOB + 3] = 0x00000e00;
    s->regs[R_DDRIOB + 4] = s->regs[R_DDRIOB + 5] = s->regs[R_DDRIOB + 6]
                          = 0x00000e00;
    s->regs[R_DDRIOB + 12] = 0x00000021;
}