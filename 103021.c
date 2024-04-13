static void vga_mem_write(void *opaque, hwaddr addr,
                          uint64_t data, unsigned size)
{
    VGACommonState *s = opaque;

    vga_mem_writeb(s, addr, data);
}
