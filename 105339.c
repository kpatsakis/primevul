static int evaluate_tpr_instruction(VAPICROMState *s, X86CPU *cpu,
                                    target_ulong *pip, TPRAccess access)
{
    CPUState *cs = CPU(cpu);
    const TPRInstruction *instr;
    target_ulong ip = *pip;
    uint8_t opcode[2];
    uint32_t real_tpr_addr;
    int i;

    if ((ip & 0xf0000000ULL) != 0x80000000ULL &&
        (ip & 0xf0000000ULL) != 0xe0000000ULL) {
        return -1;
    }

    /*
     * Early Windows 2003 SMP initialization contains a
     *
     *   mov imm32, r/m32
     *
     * instruction that is patched by TPR optimization. The problem is that
     * RSP, used by the patched instruction, is zero, so the guest gets a
     * double fault and dies.
     */
    if (cpu->env.regs[R_ESP] == 0) {
        return -1;
    }

    if (kvm_enabled() && !kvm_irqchip_in_kernel()) {
        /*
         * KVM without kernel-based TPR access reporting will pass an IP that
         * points after the accessing instruction. So we need to look backward
         * to find the reason.
         */
        for (i = 0; i < ARRAY_SIZE(tpr_instr); i++) {
            instr = &tpr_instr[i];
            if (instr->access != access) {
                continue;
            }
            if (cpu_memory_rw_debug(cs, ip - instr->length, opcode,
                                    sizeof(opcode), 0) < 0) {
                return -1;
            }
            if (opcode_matches(opcode, instr)) {
                ip -= instr->length;
                goto instruction_ok;
            }
        }
        return -1;
    } else {
        if (cpu_memory_rw_debug(cs, ip, opcode, sizeof(opcode), 0) < 0) {
            return -1;
        }
        for (i = 0; i < ARRAY_SIZE(tpr_instr); i++) {
            instr = &tpr_instr[i];
            if (opcode_matches(opcode, instr)) {
                goto instruction_ok;
            }
        }
        return -1;
    }

instruction_ok:
    /*
     * Grab the virtual TPR address from the instruction
     * and update the cached values.
     */
    if (cpu_memory_rw_debug(cs, ip + instr->addr_offset,
                            (void *)&real_tpr_addr,
                            sizeof(real_tpr_addr), 0) < 0) {
        return -1;
    }
    real_tpr_addr = le32_to_cpu(real_tpr_addr);
    if ((real_tpr_addr & 0xfff) != 0x80) {
        return -1;
    }
    s->real_tpr_addr = real_tpr_addr;
    update_guest_rom_state(s);

    *pip = ip;
    return 0;
}
