static void do_vapic_enable(void *data)
{
    VAPICROMState *s = data;
    X86CPU *cpu = X86_CPU(first_cpu);

    static const uint8_t enabled = 1;
    cpu_physical_memory_write(s->vapic_paddr + offsetof(VAPICState, enabled),
                              &enabled, sizeof(enabled));
    apic_enable_vapic(cpu->apic_state, s->vapic_paddr);
    s->state = VAPIC_ACTIVE;
}
