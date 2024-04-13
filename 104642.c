pvscsi_io_write(void *opaque, hwaddr addr,
                uint64_t val, unsigned size)
{
    PVSCSIState *s = opaque;

    switch (addr) {
    case PVSCSI_REG_OFFSET_COMMAND:
        pvscsi_on_command(s, val);
        break;

    case PVSCSI_REG_OFFSET_COMMAND_DATA:
        pvscsi_on_command_data(s, (uint32_t) val);
        break;

    case PVSCSI_REG_OFFSET_INTR_STATUS:
        trace_pvscsi_io_write("PVSCSI_REG_OFFSET_INTR_STATUS", val);
        s->reg_interrupt_status &= ~val;
        pvscsi_update_irq_status(s);
        pvscsi_schedule_completion_processing(s);
        break;

    case PVSCSI_REG_OFFSET_INTR_MASK:
        trace_pvscsi_io_write("PVSCSI_REG_OFFSET_INTR_MASK", val);
        s->reg_interrupt_enabled = val;
        pvscsi_update_irq_status(s);
        break;

    case PVSCSI_REG_OFFSET_KICK_NON_RW_IO:
        trace_pvscsi_io_write("PVSCSI_REG_OFFSET_KICK_NON_RW_IO", val);
        pvscsi_process_io(s);
        break;

    case PVSCSI_REG_OFFSET_KICK_RW_IO:
        trace_pvscsi_io_write("PVSCSI_REG_OFFSET_KICK_RW_IO", val);
        pvscsi_process_io(s);
        break;

    case PVSCSI_REG_OFFSET_DEBUG:
        trace_pvscsi_io_write("PVSCSI_REG_OFFSET_DEBUG", val);
        break;

    default:
        trace_pvscsi_io_write_unknown(addr, size, val);
        break;
    }

}
