static void write_guest_rom_state(VAPICROMState *s)
{
    cpu_physical_memory_write(s->rom_state_paddr, &s->rom_state,
                              sizeof(GuestROMState));
}
