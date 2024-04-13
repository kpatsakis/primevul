pvscsi_on_issue_scsi(PVSCSIState *s)
{
    trace_pvscsi_on_cmd_noimpl("PVSCSI_CMD_ISSUE_SCSI");
    return PVSCSI_COMMAND_PROCESSING_FAILED;
}
