static size_t vpack(uint8_t **p_data, const char *fmt, va_list ap1)
{
    size_t size = 0;
    uint8_t *data = NULL;

    if (p_data) {
        va_list ap2;

        va_copy(ap2, ap1);
        size = vfill(NULL, 0, fmt, ap2);
        *p_data = data = g_malloc(size);
        va_end(ap2);
    }
    return vfill(data, size, fmt, ap1);
}
