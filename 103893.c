static uint64_t ahci_mem_read(void *opaque, hwaddr addr, unsigned size)
{
    hwaddr aligned = addr & ~0x3;
    int ofst = addr - aligned;
    uint64_t lo = ahci_mem_read_32(opaque, aligned);
    uint64_t hi;
    uint64_t val;

    /* if < 8 byte read does not cross 4 byte boundary */
    if (ofst + size <= 4) {
        val = lo >> (ofst * 8);
    } else {
        g_assert_cmpint(size, >, 1);

        /* If the 64bit read is unaligned, we will produce undefined
         * results. AHCI does not support unaligned 64bit reads. */
        hi = ahci_mem_read_32(opaque, aligned + 4);
        val = (hi << 32 | lo) >> (ofst * 8);
    }

    DPRINTF(-1, "addr=0x%" HWADDR_PRIx " val=0x%" PRIx64 ", size=%d\n",
            addr, val, size);
    return val;
}
