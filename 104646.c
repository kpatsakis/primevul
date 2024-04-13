pvscsi_on_cmd_adapter_reset(PVSCSIState *s)
{
    trace_pvscsi_on_cmd_arrived("PVSCSI_CMD_ADAPTER_RESET");

    pvscsi_reset_adapter(s);
    return PVSCSI_COMMAND_PROCESSING_SUCCEEDED;
}
