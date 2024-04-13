static void __report_core_warn(const char *fname, struct vrend_context *ctx, enum virgl_ctx_errors error, uint32_t value)
{
   fprintf(stderr,"%s: core profile violation reported %d \"%s\" %s %d\n", fname, ctx->ctx_id, ctx->debug_name, vrend_core_profile_warn_strings[error], value);
}
