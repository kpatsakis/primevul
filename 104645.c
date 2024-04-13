pvscsi_on_cmd_abort(PVSCSIState *s)
{
    PVSCSICmdDescAbortCmd *cmd = (PVSCSICmdDescAbortCmd *) s->curr_cmd_data;
    PVSCSIRequest *r, *next;

    trace_pvscsi_on_cmd_abort(cmd->context, cmd->target);

    QTAILQ_FOREACH_SAFE(r, &s->pending_queue, next, next) {
        if (r->req.context == cmd->context) {
            break;
        }
    }
    if (r) {
        assert(!r->completed);
        r->cmp.hostStatus = BTSTAT_ABORTQUEUE;
        scsi_req_cancel(r->sreq);
    }

    return PVSCSI_COMMAND_PROCESSING_SUCCEEDED;
}
