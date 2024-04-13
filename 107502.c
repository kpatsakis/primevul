static int dstr_printf(struct dstr *str, const char *fmt, ...)
{
    va_list ap, aq;
    int len;

    va_start(ap, fmt);
    va_copy(aq, ap);
    len = vsnprintf(NULL, 0, fmt, ap);
    if (dstr_ensure(str, str->used_len + len + 1) < 0) {
        va_end(ap);
        va_end(aq);
        return -ENOMEM;
    }
    vsprintf(&str->data[str->used_len], fmt, aq);
    str->used_len += len;
    va_end(ap);
    va_end(aq);

    return len;
}
