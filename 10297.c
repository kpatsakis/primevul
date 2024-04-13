static uint64_t zynq_slcr_compute_clock(const uint64_t periods[],
                                        uint32_t ctrl_reg,
                                        unsigned index)
{
    uint32_t srcsel = extract32(ctrl_reg, 4, 2); /* bits [5:4] */
    uint32_t divisor = extract32(ctrl_reg, 8, 6); /* bits [13:8] */

    /* first, check if clock is disabled */
    if (((ctrl_reg >> index) & 1u) == 0) {
        return 0;
    }

    /*
     * according to the Zynq technical ref. manual UG585 v1.12.2 in
     * Clocks chapter, section 25.10.1 page 705:
     * "The 6-bit divider provides a divide range of 1 to 63"
     * We follow here what is implemented in linux kernel and consider
     * the 0 value as a bypass (no division).
     */
    /* frequency divisor -> period multiplication */
    return periods[srcsel] * (divisor ? divisor : 1u);
}