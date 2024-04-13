static void megasas_command_cancel(SCSIRequest *req)
{
    MegasasCmd *cmd = req->hba_private;

    if (cmd) {
        megasas_abort_command(cmd);
    } else {
        scsi_req_unref(req);
    }
}
