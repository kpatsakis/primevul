static int vapic_enable(VAPICROMState *s, X86CPU *cpu)
{
    int cpu_number = get_kpcr_number(cpu);
    hwaddr vapic_paddr;
    static const uint8_t enabled = 1;

    if (cpu_number < 0) {
        return -1;
    }
    vapic_paddr = s->vapic_paddr +
        (((hwaddr)cpu_number) << VAPIC_CPU_SHIFT);
    cpu_physical_memory_write(vapic_paddr + offsetof(VAPICState, enabled),
                              &enabled, sizeof(enabled));
    apic_enable_vapic(cpu->apic_state, vapic_paddr);

    s->state = VAPIC_ACTIVE;

    return 0;
}
