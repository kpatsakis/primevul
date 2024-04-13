static bool zynq_slcr_check_offset(hwaddr offset, bool rnw)
{
    switch (offset) {
    case R_LOCK:
    case R_UNLOCK:
    case R_DDR_CAL_START:
    case R_DDR_REF_START:
        return !rnw; /* Write only */
    case R_LOCKSTA:
    case R_FPGA0_THR_STA:
    case R_FPGA1_THR_STA:
    case R_FPGA2_THR_STA:
    case R_FPGA3_THR_STA:
    case R_BOOT_MODE:
    case R_PSS_IDCODE:
    case R_DDR_CMD_STA:
    case R_DDR_DFI_STATUS:
    case R_PLL_STATUS:
        return rnw;/* read only */
    case R_SCL:
    case R_ARM_PLL_CTRL ... R_IO_PLL_CTRL:
    case R_ARM_PLL_CFG ... R_IO_PLL_CFG:
    case R_ARM_CLK_CTRL ... R_TOPSW_CLK_CTRL:
    case R_FPGA0_CLK_CTRL ... R_FPGA0_THR_CNT:
    case R_FPGA1_CLK_CTRL ... R_FPGA1_THR_CNT:
    case R_FPGA2_CLK_CTRL ... R_FPGA2_THR_CNT:
    case R_FPGA3_CLK_CTRL ... R_FPGA3_THR_CNT:
    case R_BANDGAP_TRIP:
    case R_PLL_PREDIVISOR:
    case R_CLK_621_TRUE:
    case R_PSS_RST_CTRL ... R_A9_CPU_RST_CTRL:
    case R_RS_AWDT_CTRL:
    case R_RST_REASON:
    case R_REBOOT_STATUS:
    case R_APU_CTRL:
    case R_WDT_CLK_SEL:
    case R_TZ_DMA_NS ... R_TZ_DMA_PERIPH_NS:
    case R_DDR_URGENT:
    case R_DDR_URGENT_SEL:
    case R_MIO ... R_MIO + MIO_LENGTH - 1:
    case R_MIO_LOOPBACK ... R_MIO_MST_TRI1:
    case R_SD0_WP_CD_SEL:
    case R_SD1_WP_CD_SEL:
    case R_LVL_SHFTR_EN:
    case R_OCM_CFG:
    case R_CPU_RAM:
    case R_IOU:
    case R_DMAC_RAM:
    case R_AFI0 ... R_AFI3 + AFI_LENGTH - 1:
    case R_OCM:
    case R_DEVCI_RAM:
    case R_CSG_RAM:
    case R_GPIOB_CTRL ... R_GPIOB_CFG_CMOS33:
    case R_GPIOB_CFG_HSTL:
    case R_GPIOB_DRVR_BIAS_CTRL:
    case R_DDRIOB ... R_DDRIOB + DDRIOB_LENGTH - 1:
        return true;
    default:
        return false;
    }
}