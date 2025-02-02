void vga_init(VGACommonState *s, Object *obj, MemoryRegion *address_space,
              MemoryRegion *address_space_io, bool init_vga_ports)
{
    MemoryRegion *vga_io_memory;
    const MemoryRegionPortio *vga_ports, *vbe_ports;

    qemu_register_reset(vga_reset, s);

    s->bank_offset = 0;

    s->legacy_address_space = address_space;

    vga_io_memory = vga_init_io(s, obj, &vga_ports, &vbe_ports);
    memory_region_add_subregion_overlap(address_space,
                                        0x000a0000,
                                        vga_io_memory,
                                        1);
    memory_region_set_coalescing(vga_io_memory);
    if (init_vga_ports) {
        portio_list_init(&s->vga_port_list, obj, vga_ports, s, "vga");
        portio_list_set_flush_coalesced(&s->vga_port_list);
        portio_list_add(&s->vga_port_list, address_space_io, 0x3b0);
    }
    if (vbe_ports) {
        portio_list_init(&s->vbe_port_list, obj, vbe_ports, s, "vbe");
        portio_list_add(&s->vbe_port_list, address_space_io, 0x1ce);
    }
}
