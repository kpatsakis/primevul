static uint64_t zynq_slcr_read(void *opaque, hwaddr offset,
    unsigned size)
{
    ZynqSLCRState *s = opaque;
    offset /= 4;
    uint32_t ret = s->regs[offset];

    if (!zynq_slcr_check_offset(offset, true)) {
        qemu_log_mask(LOG_GUEST_ERROR, "zynq_slcr: Invalid read access to "
                      " addr %" HWADDR_PRIx "\n", offset * 4);
    }

    DB_PRINT("addr: %08" HWADDR_PRIx " data: %08" PRIx32 "\n", offset * 4, ret);
    return ret;
}