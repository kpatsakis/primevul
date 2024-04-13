jbig2_error(Jbig2Ctx *ctx, Jbig2Severity severity, int32_t segment_number, const char *fmt, ...)
{
    char buf[1024];
    va_list ap;
    int n;
    int code;

    va_start(ap, fmt);
    n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n < 0 || n == sizeof(buf))
        strncpy(buf, "jbig2_error: error in generating error string", sizeof(buf));
    code = ctx->error_callback(ctx->error_callback_data, buf, severity, segment_number);
    if (severity == JBIG2_SEVERITY_FATAL)
        code = -1;
    return code;
}
