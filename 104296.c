static void unmap_linear_vram(CirrusVGAState *s)
{
    if (s->bustype == CIRRUS_BUSTYPE_PCI && s->linear_vram) {
        s->linear_vram = false;
        memory_region_del_subregion(&s->pci_bar, &s->vga.vram);
    }
    memory_region_set_enabled(&s->cirrus_bank[0], false);
    memory_region_set_enabled(&s->cirrus_bank[1], false);
}
