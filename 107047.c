static CCPrepare gen_prepare_eflags_z(DisasContext *s, TCGv reg)
{
    switch (s->cc_op) {
    case CC_OP_DYNAMIC:
        gen_compute_eflags(s);
        /* FALLTHRU */
    case CC_OP_EFLAGS:
    case CC_OP_ADCX:
    case CC_OP_ADOX:
    case CC_OP_ADCOX:
        return (CCPrepare) { .cond = TCG_COND_NE, .reg = cpu_cc_src,
                             .mask = CC_Z };
    case CC_OP_CLR:
        return (CCPrepare) { .cond = TCG_COND_ALWAYS, .mask = -1 };
    case CC_OP_POPCNT:
        return (CCPrepare) { .cond = TCG_COND_EQ, .reg = cpu_cc_src,
                             .mask = -1 };
    default:
        {
            TCGMemOp size = (s->cc_op - CC_OP_ADDB) & 3;
            TCGv t0 = gen_ext_tl(reg, cpu_cc_dst, size, false);
            return (CCPrepare) { .cond = TCG_COND_EQ, .reg = t0, .mask = -1 };
        }
    }
}
