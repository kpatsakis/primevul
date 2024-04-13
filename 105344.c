static void patch_byte(X86CPU *cpu, target_ulong addr, uint8_t byte)
{
    cpu_memory_rw_debug(CPU(cpu), addr, &byte, 1, 1);
}
