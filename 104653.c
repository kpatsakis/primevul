pvscsi_on_cmd_unplug(PVSCSIState *s)
{
    trace_pvscsi_on_cmd_noimpl("PVSCSI_CMD_DEVICE_UNPLUG");
    return PVSCSI_COMMAND_PROCESSING_FAILED;
}
