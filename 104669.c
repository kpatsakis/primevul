pvscsi_reset_state(PVSCSIState *s)
{
    s->curr_cmd = PVSCSI_CMD_FIRST;
    s->curr_cmd_data_cntr = 0;
    s->reg_command_status = PVSCSI_COMMAND_PROCESSING_SUCCEEDED;
    s->reg_interrupt_status = 0;
    pvscsi_ring_cleanup(&s->rings);
    s->rings_info_valid = FALSE;
    s->msg_ring_info_valid = FALSE;
    QTAILQ_INIT(&s->pending_queue);
    QTAILQ_INIT(&s->completion_queue);
}
