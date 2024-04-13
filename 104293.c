static void map_linear_vram(CirrusVGAState *s)
{
    if (s->bustype == CIRRUS_BUSTYPE_PCI && !s->linear_vram) {
        s->linear_vram = true;
        memory_region_add_subregion_overlap(&s->pci_bar, 0, &s->vga.vram, 1);
    }
    map_linear_vram_bank(s, 0);
    map_linear_vram_bank(s, 1);
}
