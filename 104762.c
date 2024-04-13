static size_t fill(uint8_t *data, size_t size, const char *fmt, ...)
{
    va_list ap;
    size_t ret;

    va_start(ap, fmt);
    ret = vfill(data, size, fmt, ap);
    va_end(ap);

    return ret;
}
