static void zynq_slcr_write(void *opaque, hwaddr offset,
                          uint64_t val, unsigned size)
{
    ZynqSLCRState *s = (ZynqSLCRState *)opaque;
    offset /= 4;

    DB_PRINT("addr: %08" HWADDR_PRIx " data: %08" PRIx64 "\n", offset * 4, val);

    if (!zynq_slcr_check_offset(offset, false)) {
        qemu_log_mask(LOG_GUEST_ERROR, "zynq_slcr: Invalid write access to "
                      "addr %" HWADDR_PRIx "\n", offset * 4);
        return;
    }

    switch (offset) {
    case R_SCL:
        s->regs[R_SCL] = val & 0x1;
        return;
    case R_LOCK:
        if ((val & 0xFFFF) == XILINX_LOCK_KEY) {
            DB_PRINT("XILINX LOCK 0xF8000000 + 0x%x <= 0x%x\n", (int)offset,
                (unsigned)val & 0xFFFF);
            s->regs[R_LOCKSTA] = 1;
        } else {
            DB_PRINT("WRONG XILINX LOCK KEY 0xF8000000 + 0x%x <= 0x%x\n",
                (int)offset, (unsigned)val & 0xFFFF);
        }
        return;
    case R_UNLOCK:
        if ((val & 0xFFFF) == XILINX_UNLOCK_KEY) {
            DB_PRINT("XILINX UNLOCK 0xF8000000 + 0x%x <= 0x%x\n", (int)offset,
                (unsigned)val & 0xFFFF);
            s->regs[R_LOCKSTA] = 0;
        } else {
            DB_PRINT("WRONG XILINX UNLOCK KEY 0xF8000000 + 0x%x <= 0x%x\n",
                (int)offset, (unsigned)val & 0xFFFF);
        }
        return;
    }

    if (s->regs[R_LOCKSTA]) {
        qemu_log_mask(LOG_GUEST_ERROR,
                      "SCLR registers are locked. Unlock them first\n");
        return;
    }
    s->regs[offset] = val;

    switch (offset) {
    case R_PSS_RST_CTRL:
        if (FIELD_EX32(val, PSS_RST_CTRL, SOFT_RST)) {
            qemu_system_reset_request(SHUTDOWN_CAUSE_GUEST_RESET);
        }
        break;
    case R_IO_PLL_CTRL:
    case R_ARM_PLL_CTRL:
    case R_DDR_PLL_CTRL:
    case R_UART_CLK_CTRL:
        zynq_slcr_compute_clocks(s);
        zynq_slcr_propagate_clocks(s);
        break;
    }
}