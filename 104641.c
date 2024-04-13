pvscsi_io_read(void *opaque, hwaddr addr, unsigned size)
{
    PVSCSIState *s = opaque;

    switch (addr) {
    case PVSCSI_REG_OFFSET_INTR_STATUS:
        trace_pvscsi_io_read("PVSCSI_REG_OFFSET_INTR_STATUS",
                             s->reg_interrupt_status);
        return s->reg_interrupt_status;

    case PVSCSI_REG_OFFSET_INTR_MASK:
        trace_pvscsi_io_read("PVSCSI_REG_OFFSET_INTR_MASK",
                             s->reg_interrupt_status);
        return s->reg_interrupt_enabled;

    case PVSCSI_REG_OFFSET_COMMAND_STATUS:
        trace_pvscsi_io_read("PVSCSI_REG_OFFSET_COMMAND_STATUS",
                             s->reg_interrupt_status);
        return s->reg_command_status;

    default:
        trace_pvscsi_io_read_unknown(addr, size);
        return 0;
    }
}
