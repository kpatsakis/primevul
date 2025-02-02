static void gen_rot_rm_T1(DisasContext *s, TCGMemOp ot, int op1, int is_right)
{
    target_ulong mask = (ot == MO_64 ? 0x3f : 0x1f);
    TCGv_i32 t0, t1;

    /* load */
    if (op1 == OR_TMP0) {
        gen_op_ld_v(s, ot, cpu_T0, cpu_A0);
    } else {
        gen_op_mov_v_reg(ot, cpu_T0, op1);
    }

    tcg_gen_andi_tl(cpu_T1, cpu_T1, mask);

    switch (ot) {
    case MO_8:
        /* Replicate the 8-bit input so that a 32-bit rotate works.  */
        tcg_gen_ext8u_tl(cpu_T0, cpu_T0);
        tcg_gen_muli_tl(cpu_T0, cpu_T0, 0x01010101);
        goto do_long;
    case MO_16:
        /* Replicate the 16-bit input so that a 32-bit rotate works.  */
        tcg_gen_deposit_tl(cpu_T0, cpu_T0, cpu_T0, 16, 16);
        goto do_long;
    do_long:
#ifdef TARGET_X86_64
    case MO_32:
        tcg_gen_trunc_tl_i32(cpu_tmp2_i32, cpu_T0);
        tcg_gen_trunc_tl_i32(cpu_tmp3_i32, cpu_T1);
        if (is_right) {
            tcg_gen_rotr_i32(cpu_tmp2_i32, cpu_tmp2_i32, cpu_tmp3_i32);
        } else {
            tcg_gen_rotl_i32(cpu_tmp2_i32, cpu_tmp2_i32, cpu_tmp3_i32);
        }
        tcg_gen_extu_i32_tl(cpu_T0, cpu_tmp2_i32);
        break;
#endif
    default:
        if (is_right) {
            tcg_gen_rotr_tl(cpu_T0, cpu_T0, cpu_T1);
        } else {
            tcg_gen_rotl_tl(cpu_T0, cpu_T0, cpu_T1);
        }
        break;
    }

    /* store */
    gen_op_st_rm_T0_A0(s, ot, op1);

    /* We'll need the flags computed into CC_SRC.  */
    gen_compute_eflags(s);

    /* The value that was "rotated out" is now present at the other end
       of the word.  Compute C into CC_DST and O into CC_SRC2.  Note that
       since we've computed the flags into CC_SRC, these variables are
       currently dead.  */
    if (is_right) {
        tcg_gen_shri_tl(cpu_cc_src2, cpu_T0, mask - 1);
        tcg_gen_shri_tl(cpu_cc_dst, cpu_T0, mask);
        tcg_gen_andi_tl(cpu_cc_dst, cpu_cc_dst, 1);
    } else {
        tcg_gen_shri_tl(cpu_cc_src2, cpu_T0, mask);
        tcg_gen_andi_tl(cpu_cc_dst, cpu_T0, 1);
    }
    tcg_gen_andi_tl(cpu_cc_src2, cpu_cc_src2, 1);
    tcg_gen_xor_tl(cpu_cc_src2, cpu_cc_src2, cpu_cc_dst);

    /* Now conditionally store the new CC_OP value.  If the shift count
       is 0 we keep the CC_OP_EFLAGS setting so that only CC_SRC is live.
       Otherwise reuse CC_OP_ADCOX which have the C and O flags split out
       exactly as we computed above.  */
    t0 = tcg_const_i32(0);
    t1 = tcg_temp_new_i32();
    tcg_gen_trunc_tl_i32(t1, cpu_T1);
    tcg_gen_movi_i32(cpu_tmp2_i32, CC_OP_ADCOX); 
    tcg_gen_movi_i32(cpu_tmp3_i32, CC_OP_EFLAGS);
    tcg_gen_movcond_i32(TCG_COND_NE, cpu_cc_op, t1, t0,
                        cpu_tmp2_i32, cpu_tmp3_i32);
    tcg_temp_free_i32(t0);
    tcg_temp_free_i32(t1);

    /* The CC_OP value is no longer predictable.  */ 
    set_cc_op(s, CC_OP_DYNAMIC);
}
