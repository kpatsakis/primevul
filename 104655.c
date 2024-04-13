pvscsi_on_command_data(PVSCSIState *s, uint32_t value)
{
    size_t bytes_arrived = s->curr_cmd_data_cntr * sizeof(uint32_t);

    assert(bytes_arrived < sizeof(s->curr_cmd_data));
    s->curr_cmd_data[s->curr_cmd_data_cntr++] = value;

    pvscsi_do_command_processing(s);
}
