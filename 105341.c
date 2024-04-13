static void kvmvapic_vm_state_change(void *opaque, int running,
                                     RunState state)
{
    VAPICROMState *s = opaque;
    uint8_t *zero;

    if (!running) {
        return;
    }

    if (s->state == VAPIC_ACTIVE) {
        if (smp_cpus == 1) {
            run_on_cpu(first_cpu, do_vapic_enable, s);
        } else {
            zero = g_malloc0(s->rom_state.vapic_size);
            cpu_physical_memory_write(s->vapic_paddr, zero,
                                      s->rom_state.vapic_size);
            g_free(zero);
        }
    }

    qemu_del_vm_change_state_handler(s->vmsentry);
}
