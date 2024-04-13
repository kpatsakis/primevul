static void isa_cirrus_vga_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->vmsd  = &vmstate_cirrus_vga;
    dc->realize = isa_cirrus_vga_realizefn;
    dc->props = isa_cirrus_vga_properties;
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}
