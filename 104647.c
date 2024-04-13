pvscsi_on_cmd_config(PVSCSIState *s)
{
    trace_pvscsi_on_cmd_noimpl("PVSCSI_CMD_CONFIG");
    return PVSCSI_COMMAND_PROCESSING_FAILED;
}
