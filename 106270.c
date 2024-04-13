static void cirrus_init_common(CirrusVGAState *s, Object *owner,
                               int device_id, int is_pci,
                               MemoryRegion *system_memory,
                               MemoryRegion *system_io)
{
    int i;
    static int inited;

    if (!inited) {
        inited = 1;
        for(i = 0;i < 256; i++)
            rop_to_index[i] = CIRRUS_ROP_NOP_INDEX; /* nop rop */
        rop_to_index[CIRRUS_ROP_0] = 0;
        rop_to_index[CIRRUS_ROP_SRC_AND_DST] = 1;
        rop_to_index[CIRRUS_ROP_NOP] = 2;
        rop_to_index[CIRRUS_ROP_SRC_AND_NOTDST] = 3;
        rop_to_index[CIRRUS_ROP_NOTDST] = 4;
        rop_to_index[CIRRUS_ROP_SRC] = 5;
        rop_to_index[CIRRUS_ROP_1] = 6;
        rop_to_index[CIRRUS_ROP_NOTSRC_AND_DST] = 7;
        rop_to_index[CIRRUS_ROP_SRC_XOR_DST] = 8;
        rop_to_index[CIRRUS_ROP_SRC_OR_DST] = 9;
        rop_to_index[CIRRUS_ROP_NOTSRC_OR_NOTDST] = 10;
        rop_to_index[CIRRUS_ROP_SRC_NOTXOR_DST] = 11;
        rop_to_index[CIRRUS_ROP_SRC_OR_NOTDST] = 12;
        rop_to_index[CIRRUS_ROP_NOTSRC] = 13;
        rop_to_index[CIRRUS_ROP_NOTSRC_OR_DST] = 14;
        rop_to_index[CIRRUS_ROP_NOTSRC_AND_NOTDST] = 15;
        s->device_id = device_id;
        if (is_pci)
            s->bustype = CIRRUS_BUSTYPE_PCI;
        else
            s->bustype = CIRRUS_BUSTYPE_ISA;
    }

    /* Register ioport 0x3b0 - 0x3df */
    memory_region_init_io(&s->cirrus_vga_io, owner, &cirrus_vga_io_ops, s,
                          "cirrus-io", 0x30);
    memory_region_set_flush_coalesced(&s->cirrus_vga_io);
    memory_region_add_subregion(system_io, 0x3b0, &s->cirrus_vga_io);

    memory_region_init(&s->low_mem_container, owner,
                       "cirrus-lowmem-container",
                       0x20000);

    memory_region_init_io(&s->low_mem, owner, &cirrus_vga_mem_ops, s,
                          "cirrus-low-memory", 0x20000);
    memory_region_add_subregion(&s->low_mem_container, 0, &s->low_mem);
    for (i = 0; i < 2; ++i) {
        static const char *names[] = { "vga.bank0", "vga.bank1" };
        MemoryRegion *bank = &s->cirrus_bank[i];
        memory_region_init_alias(bank, owner, names[i], &s->vga.vram,
                                 0, 0x8000);
        memory_region_set_enabled(bank, false);
        memory_region_add_subregion_overlap(&s->low_mem_container, i * 0x8000,
                                            bank, 1);
    }
    memory_region_add_subregion_overlap(system_memory,
                                        0x000a0000,
                                        &s->low_mem_container,
                                        1);
    memory_region_set_coalescing(&s->low_mem);

    /* I/O handler for LFB */
    memory_region_init_io(&s->cirrus_linear_io, owner, &cirrus_linear_io_ops, s,
                          "cirrus-linear-io", s->vga.vram_size_mb
                                              * 1024 * 1024);
    memory_region_set_flush_coalesced(&s->cirrus_linear_io);

    /* I/O handler for LFB */
    memory_region_init_io(&s->cirrus_linear_bitblt_io, owner,
                          &cirrus_linear_bitblt_io_ops,
                          s,
                          "cirrus-bitblt-mmio",
                          0x400000);
    memory_region_set_flush_coalesced(&s->cirrus_linear_bitblt_io);

    /* I/O handler for memory-mapped I/O */
    memory_region_init_io(&s->cirrus_mmio_io, owner, &cirrus_mmio_io_ops, s,
                          "cirrus-mmio", CIRRUS_PNPMMIO_SIZE);
    memory_region_set_flush_coalesced(&s->cirrus_mmio_io);

    s->real_vram_size =
        (s->device_id == CIRRUS_ID_CLGD5446) ? 4096 * 1024 : 2048 * 1024;

    /* XXX: s->vga.vram_size must be a power of two */
    s->cirrus_addr_mask = s->real_vram_size - 1;
    s->linear_mmio_mask = s->real_vram_size - 256;

    s->vga.get_bpp = cirrus_get_bpp;
    s->vga.get_offsets = cirrus_get_offsets;
    s->vga.get_resolution = cirrus_get_resolution;
    s->vga.cursor_invalidate = cirrus_cursor_invalidate;
    s->vga.cursor_draw_line = cirrus_cursor_draw_line;

    qemu_register_reset(cirrus_reset, s);
}
