static uint64_t zynq_slcr_compute_pll(uint64_t input, uint32_t ctrl_reg)
{
    uint32_t mult = ((ctrl_reg & R_xxx_PLL_CTRL_PLL_FPDIV_MASK) >>
            R_xxx_PLL_CTRL_PLL_FPDIV_SHIFT);

    /* first, check if pll is bypassed */
    if (ctrl_reg & R_xxx_PLL_CTRL_PLL_BYPASS_FORCE_MASK) {
        return input;
    }

    /* is pll disabled ? */
    if (ctrl_reg & (R_xxx_PLL_CTRL_PLL_RESET_MASK |
                    R_xxx_PLL_CTRL_PLL_PWRDWN_MASK)) {
        return 0;
    }

    /* Consider zero feedback as maximum divide ratio possible */
    if (!mult) {
        mult = 1 << R_xxx_PLL_CTRL_PLL_FPDIV_LENGTH;
    }

    /* frequency multiplier -> period division */
    return input / mult;
}