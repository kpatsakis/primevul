static void cirrus_mmio_write(void *opaque, hwaddr addr,
                              uint64_t val, unsigned size)
{
    CirrusVGAState *s = opaque;

    if (addr >= 0x100) {
	cirrus_mmio_blt_write(s, addr - 0x100, val);
    } else {
        cirrus_vga_ioport_write(s, addr + 0x10, val, size);
    }
}
