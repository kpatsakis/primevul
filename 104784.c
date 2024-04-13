static size_t mptsas_config_pack(uint8_t **data, const char *fmt, ...)
{
    va_list ap;
    size_t ret;

    va_start(ap, fmt);
    ret = vpack(data, fmt, ap);
    va_end(ap);

    if (data) {
        assert(ret / 4 < 256 && (ret % 4) == 0);
        stb_p(*data + 1, ret / 4);
    }
    return ret;
}
