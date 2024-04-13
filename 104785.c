static size_t mptsas_config_pack_ext(uint8_t **data, const char *fmt, ...)
{
    va_list ap;
    size_t ret;

    va_start(ap, fmt);
    ret = vpack(data, fmt, ap);
    va_end(ap);

    if (data) {
        assert(ret < 65536 && (ret % 4) == 0);
        stw_le_p(*data + 4, ret / 4);
    }
    return ret;
}
