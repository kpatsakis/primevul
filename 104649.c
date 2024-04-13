pvscsi_on_cmd_reset_device(PVSCSIState *s)
{
    uint8_t target_lun = 0;
    struct PVSCSICmdDescResetDevice *cmd =
        (struct PVSCSICmdDescResetDevice *) s->curr_cmd_data;
    SCSIDevice *sdev;

    sdev = pvscsi_device_find(s, 0, cmd->target, cmd->lun, &target_lun);

    trace_pvscsi_on_cmd_reset_dev(cmd->target, (int) target_lun, sdev);

    if (sdev != NULL) {
        s->resetting++;
        device_reset(&sdev->qdev);
        s->resetting--;
        return PVSCSI_COMMAND_PROCESSING_SUCCEEDED;
    }

    return PVSCSI_COMMAND_PROCESSING_FAILED;
}
