pvscsi_on_command(PVSCSIState *s, uint64_t cmd_id)
{
    if ((cmd_id > PVSCSI_CMD_FIRST) && (cmd_id < PVSCSI_CMD_LAST)) {
        s->curr_cmd = cmd_id;
    } else {
        s->curr_cmd = PVSCSI_CMD_FIRST;
        trace_pvscsi_on_cmd_unknown(cmd_id);
    }

    s->curr_cmd_data_cntr = 0;
    s->reg_command_status = PVSCSI_COMMAND_NOT_ENOUGH_DATA;

    pvscsi_do_command_processing(s);
}
