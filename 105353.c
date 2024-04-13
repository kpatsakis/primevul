static void vapic_enable_tpr_reporting(bool enable)
{
    VAPICEnableTPRReporting info = {
        .enable = enable,
    };
    CPUState *cs;
    X86CPU *cpu;

    CPU_FOREACH(cs) {
        cpu = X86_CPU(cs);
        info.apic = cpu->apic_state;
        run_on_cpu(cs, vapic_do_enable_tpr_reporting, &info);
    }
}
