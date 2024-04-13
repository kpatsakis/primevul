static void lsi_memcpy(LSIState *s, uint32_t dest, uint32_t src, int count)
{
    int n;
    uint8_t buf[LSI_BUF_SIZE];

    trace_lsi_memcpy(dest, src, count);
    while (count) {
        n = (count > LSI_BUF_SIZE) ? LSI_BUF_SIZE : count;
        lsi_mem_read(s, src, buf, n);
        lsi_mem_write(s, dest, buf, n);
        src += n;
        dest += n;
        count -= n;
    }
}
