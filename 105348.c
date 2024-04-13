static void update_guest_rom_state(VAPICROMState *s)
{
    read_guest_rom_state(s);

    s->rom_state.real_tpr_addr = cpu_to_le32(s->real_tpr_addr);
    s->rom_state.vcpu_shift = cpu_to_le32(VAPIC_CPU_SHIFT);

    write_guest_rom_state(s);
}
