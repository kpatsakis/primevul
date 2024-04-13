static int vga_get_bpp(VGACommonState *s)
{
    int ret;

    if (vbe_enabled(s)) {
        ret = s->vbe_regs[VBE_DISPI_INDEX_BPP];
    } else {
        ret = 0;
    }
    return ret;
}
