pvscsi_on_cmd_unknown(PVSCSIState *s)
{
    trace_pvscsi_on_cmd_unknown_data(s->curr_cmd_data[0]);
    return PVSCSI_COMMAND_PROCESSING_FAILED;
}
