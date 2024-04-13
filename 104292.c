static void isa_cirrus_vga_realizefn(DeviceState *dev, Error **errp)
{
    ISADevice *isadev = ISA_DEVICE(dev);
    ISACirrusVGAState *d = ISA_CIRRUS_VGA(dev);
    VGACommonState *s = &d->cirrus_vga.vga;

    /* follow real hardware, cirrus card emulated has 4 MB video memory.
       Also accept 8 MB/16 MB for backward compatibility. */
    if (s->vram_size_mb != 4 && s->vram_size_mb != 8 &&
        s->vram_size_mb != 16) {
        error_setg(errp, "Invalid cirrus_vga ram size '%u'",
                   s->vram_size_mb);
        return;
    }
    vga_common_init(s, OBJECT(dev), true);
    cirrus_init_common(&d->cirrus_vga, OBJECT(dev), CIRRUS_ID_CLGD5430, 0,
                       isa_address_space(isadev),
                       isa_address_space_io(isadev));
    s->con = graphic_console_init(dev, 0, s->hw_ops, s);
    rom_add_vga(VGABIOS_CIRRUS_FILENAME);
    /* XXX ISA-LFB support */
    /* FIXME not qdev yet */
}
