static void megasas_abort_command(MegasasCmd *cmd)
{
    if (cmd->req) {
        scsi_req_cancel(cmd->req);
        cmd->req = NULL;
    }
}
