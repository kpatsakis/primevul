void vrend_report_buffer_error(struct vrend_context *ctx, int cmd)
{
   report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_CMD_BUFFER, cmd);
}
