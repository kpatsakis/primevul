pvscsi_do_command_processing(PVSCSIState *s)
{
    size_t bytes_arrived = s->curr_cmd_data_cntr * sizeof(uint32_t);

    assert(s->curr_cmd < PVSCSI_CMD_LAST);
    if (bytes_arrived >= pvscsi_commands[s->curr_cmd].data_size) {
        s->reg_command_status = pvscsi_commands[s->curr_cmd].handler_fn(s);
        s->curr_cmd = PVSCSI_CMD_FIRST;
        s->curr_cmd_data_cntr   = 0;
    }
}
