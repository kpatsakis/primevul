static void gen_op_update_neg_cc(void)
{
    tcg_gen_mov_tl(cpu_cc_dst, cpu_T0);
    tcg_gen_neg_tl(cpu_cc_src, cpu_T0);
    tcg_gen_movi_tl(cpu_cc_srcT, 0);
}
