pvscsi_on_cmd_reset_bus(PVSCSIState *s)
{
    trace_pvscsi_on_cmd_arrived("PVSCSI_CMD_RESET_BUS");

    s->resetting++;
    qbus_reset_all_fn(&s->bus);
    s->resetting--;
    return PVSCSI_COMMAND_PROCESSING_SUCCEEDED;
}
