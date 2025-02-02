void gen_intermediate_code(CPUX86State *env, TranslationBlock *tb)
{
    X86CPU *cpu = x86_env_get_cpu(env);
    CPUState *cs = CPU(cpu);
    DisasContext dc1, *dc = &dc1;
    target_ulong pc_ptr;
    uint32_t flags;
    target_ulong pc_start;
    target_ulong cs_base;
    int num_insns;
    int max_insns;

    /* generate intermediate code */
    pc_start = tb->pc;
    cs_base = tb->cs_base;
    flags = tb->flags;

    dc->pe = (flags >> HF_PE_SHIFT) & 1;
    dc->code32 = (flags >> HF_CS32_SHIFT) & 1;
    dc->ss32 = (flags >> HF_SS32_SHIFT) & 1;
    dc->addseg = (flags >> HF_ADDSEG_SHIFT) & 1;
    dc->f_st = 0;
    dc->vm86 = (flags >> VM_SHIFT) & 1;
    dc->cpl = (flags >> HF_CPL_SHIFT) & 3;
    dc->iopl = (flags >> IOPL_SHIFT) & 3;
    dc->tf = (flags >> TF_SHIFT) & 1;
    dc->singlestep_enabled = cs->singlestep_enabled;
    dc->cc_op = CC_OP_DYNAMIC;
    dc->cc_op_dirty = false;
    dc->cs_base = cs_base;
    dc->tb = tb;
    dc->popl_esp_hack = 0;
    /* select memory access functions */
    dc->mem_index = 0;
#ifdef CONFIG_SOFTMMU
    dc->mem_index = cpu_mmu_index(env, false);
#endif
    dc->cpuid_features = env->features[FEAT_1_EDX];
    dc->cpuid_ext_features = env->features[FEAT_1_ECX];
    dc->cpuid_ext2_features = env->features[FEAT_8000_0001_EDX];
    dc->cpuid_ext3_features = env->features[FEAT_8000_0001_ECX];
    dc->cpuid_7_0_ebx_features = env->features[FEAT_7_0_EBX];
    dc->cpuid_xsave_features = env->features[FEAT_XSAVE];
#ifdef TARGET_X86_64
    dc->lma = (flags >> HF_LMA_SHIFT) & 1;
    dc->code64 = (flags >> HF_CS64_SHIFT) & 1;
#endif
    dc->flags = flags;
    dc->jmp_opt = !(dc->tf || cs->singlestep_enabled ||
                    (flags & HF_INHIBIT_IRQ_MASK));
    /* Do not optimize repz jumps at all in icount mode, because
       rep movsS instructions are execured with different paths
       in !repz_opt and repz_opt modes. The first one was used
       always except single step mode. And this setting
       disables jumps optimization and control paths become
       equivalent in run and single step modes.
       Now there will be no jump optimization for repz in
       record/replay modes and there will always be an
       additional step for ecx=0 when icount is enabled.
     */
    dc->repz_opt = !dc->jmp_opt && !(tb->cflags & CF_USE_ICOUNT);
#if 0
    /* check addseg logic */
    if (!dc->addseg && (dc->vm86 || !dc->pe || !dc->code32))
        printf("ERROR addseg\n");
#endif

    cpu_T0 = tcg_temp_new();
    cpu_T1 = tcg_temp_new();
    cpu_A0 = tcg_temp_new();

    cpu_tmp0 = tcg_temp_new();
    cpu_tmp1_i64 = tcg_temp_new_i64();
    cpu_tmp2_i32 = tcg_temp_new_i32();
    cpu_tmp3_i32 = tcg_temp_new_i32();
    cpu_tmp4 = tcg_temp_new();
    cpu_ptr0 = tcg_temp_new_ptr();
    cpu_ptr1 = tcg_temp_new_ptr();
    cpu_cc_srcT = tcg_temp_local_new();

    dc->is_jmp = DISAS_NEXT;
    pc_ptr = pc_start;
    num_insns = 0;
    max_insns = tb->cflags & CF_COUNT_MASK;
    if (max_insns == 0) {
        max_insns = CF_COUNT_MASK;
    }
    if (max_insns > TCG_MAX_INSNS) {
        max_insns = TCG_MAX_INSNS;
    }

    gen_tb_start(tb);
    for(;;) {
        tcg_gen_insn_start(pc_ptr, dc->cc_op);
        num_insns++;

        /* If RF is set, suppress an internally generated breakpoint.  */
        if (unlikely(cpu_breakpoint_test(cs, pc_ptr,
                                         tb->flags & HF_RF_MASK
                                         ? BP_GDB : BP_ANY))) {
            gen_debug(dc, pc_ptr - dc->cs_base);
            /* The address covered by the breakpoint must be included in
               [tb->pc, tb->pc + tb->size) in order to for it to be
               properly cleared -- thus we increment the PC here so that
               the logic setting tb->size below does the right thing.  */
            pc_ptr += 1;
            goto done_generating;
        }
        if (num_insns == max_insns && (tb->cflags & CF_LAST_IO)) {
            gen_io_start();
        }

        pc_ptr = disas_insn(env, dc, pc_ptr);
        /* stop translation if indicated */
        if (dc->is_jmp)
            break;
        /* if single step mode, we generate only one instruction and
           generate an exception */
        /* if irq were inhibited with HF_INHIBIT_IRQ_MASK, we clear
           the flag and abort the translation to give the irqs a
           change to be happen */
        if (dc->tf || dc->singlestep_enabled ||
            (flags & HF_INHIBIT_IRQ_MASK)) {
            gen_jmp_im(pc_ptr - dc->cs_base);
            gen_eob(dc);
            break;
        }
        /* Do not cross the boundary of the pages in icount mode,
           it can cause an exception. Do it only when boundary is
           crossed by the first instruction in the block.
           If current instruction already crossed the bound - it's ok,
           because an exception hasn't stopped this code.
         */
        if ((tb->cflags & CF_USE_ICOUNT)
            && ((pc_ptr & TARGET_PAGE_MASK)
                != ((pc_ptr + TARGET_MAX_INSN_SIZE - 1) & TARGET_PAGE_MASK)
                || (pc_ptr & ~TARGET_PAGE_MASK) == 0)) {
            gen_jmp_im(pc_ptr - dc->cs_base);
            gen_eob(dc);
            break;
        }
        /* if too long translation, stop generation too */
        if (tcg_op_buf_full() ||
            (pc_ptr - pc_start) >= (TARGET_PAGE_SIZE - 32) ||
            num_insns >= max_insns) {
            gen_jmp_im(pc_ptr - dc->cs_base);
            gen_eob(dc);
            break;
        }
        if (singlestep) {
            gen_jmp_im(pc_ptr - dc->cs_base);
            gen_eob(dc);
            break;
        }
    }
    if (tb->cflags & CF_LAST_IO)
        gen_io_end();
done_generating:
    gen_tb_end(tb, num_insns);

#ifdef DEBUG_DISAS
    if (qemu_loglevel_mask(CPU_LOG_TB_IN_ASM)
        && qemu_log_in_addr_range(pc_start)) {
        int disas_flags;
        qemu_log_lock();
        qemu_log("----------------\n");
        qemu_log("IN: %s\n", lookup_symbol(pc_start));
#ifdef TARGET_X86_64
        if (dc->code64)
            disas_flags = 2;
        else
#endif
            disas_flags = !dc->code32;
        log_target_disas(cs, pc_start, pc_ptr - pc_start, disas_flags);
        qemu_log("\n");
        qemu_log_unlock();
    }
#endif

    tb->size = pc_ptr - pc_start;
    tb->icount = num_insns;
}
