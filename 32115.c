static void __report_context_error(const char *fname, struct vrend_context *ctx, enum virgl_ctx_errors error, uint32_t value)
{
   ctx->in_error = true;
   ctx->last_error = error;
   fprintf(stderr,"%s: context error reported %d \"%s\" %s %d\n", fname, ctx->ctx_id, ctx->debug_name, vrend_ctx_error_strings[error], value);
}
