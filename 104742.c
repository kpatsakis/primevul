static uint64_t vmsvga_io_read(void *opaque, hwaddr addr, unsigned size)
{
    struct vmsvga_state_s *s = opaque;

    switch (addr) {
    case SVGA_IO_MUL * SVGA_INDEX_PORT: return vmsvga_index_read(s, addr);
    case SVGA_IO_MUL * SVGA_VALUE_PORT: return vmsvga_value_read(s, addr);
    case SVGA_IO_MUL * SVGA_BIOS_PORT: return vmsvga_bios_read(s, addr);
    default: return -1u;
    }
}
