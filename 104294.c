static void map_linear_vram_bank(CirrusVGAState *s, unsigned bank)
{
    MemoryRegion *mr = &s->cirrus_bank[bank];
    bool enabled = !(s->cirrus_srcptr != s->cirrus_srcptr_end)
        && !((s->vga.sr[0x07] & 0x01) == 0)
        && !((s->vga.gr[0x0B] & 0x14) == 0x14)
        && !(s->vga.gr[0x0B] & 0x02);

    memory_region_set_enabled(mr, enabled);
    memory_region_set_alias_offset(mr, s->cirrus_bank_base[bank]);
}
