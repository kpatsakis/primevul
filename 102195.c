void ih264d_unpack_chroma_coeff4x4_mb(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info)
{
    UWORD8 u1_mb_type = ps_cur_mb_info->u1_mb_type;
    UWORD16 ui2_chroma_csbp = ps_cur_mb_info->u2_chroma_csbp;
    UWORD8 *pu1_inv_scan = ps_dec->pu1_inv_scan;
    WORD16 *pi2_coeff_data = ps_dec->pi2_coeff_data;
    WORD32 i;
    WORD16 *pi2_dc_val_u = NULL;
    WORD16 *pi2_dc_val_v = NULL;

    PROFILE_DISABLE_UNPACK_CHROMA()
 if((ps_cur_mb_info->u1_cbp >> 4) == CBPC_ALLZERO)
 return;

 /*
     * Reserve the pointers to dc vals. The dc vals will be copied
     * after unpacking of ac vals since memset to 0 inside.
     */
 if(CHECKBIT(ps_cur_mb_info->u1_yuv_dc_block_flag,1))
 {
        pi2_dc_val_u = (WORD16 *)ps_dec->pv_proc_tu_coeff_data;

        ps_dec->pv_proc_tu_coeff_data = (void *)(pi2_dc_val_u + 4);
 }
 if(CHECKBIT(ps_cur_mb_info->u1_yuv_dc_block_flag,2))
 {
        pi2_dc_val_v = (WORD16 *)ps_dec->pv_proc_tu_coeff_data;

        ps_dec->pv_proc_tu_coeff_data = (void *)(pi2_dc_val_v + 4);
 }

 if((ps_cur_mb_info->u1_cbp >> 4) == CBPC_NONZERO)
 {
        pi2_coeff_data = ps_dec->pi2_coeff_data;
        ih264d_unpack_coeff4x4_8x8blk_chroma(ps_dec,
                                             ps_cur_mb_info,
                                             ui2_chroma_csbp,
                                             pi2_coeff_data);

        pi2_coeff_data += 64;
        ui2_chroma_csbp = ui2_chroma_csbp >> 4;
        ih264d_unpack_coeff4x4_8x8blk_chroma(ps_dec,
                                             ps_cur_mb_info,
                                             ui2_chroma_csbp,
                                             pi2_coeff_data);

 }

    pi2_coeff_data = ps_dec->pi2_coeff_data;
 if(pi2_dc_val_u != NULL)
 {
        pi2_coeff_data[0] = *pi2_dc_val_u++;
        pi2_coeff_data[1 * 16] = *pi2_dc_val_u++;
        pi2_coeff_data[2 * 16] = *pi2_dc_val_u++;
        pi2_coeff_data[3 * 16] = *pi2_dc_val_u++;
 }
 else
 {
        pi2_coeff_data[0] = 0;
        pi2_coeff_data[1 * 16] = 0;
        pi2_coeff_data[2 * 16] = 0;
        pi2_coeff_data[3 * 16] = 0;
 }
    pi2_coeff_data += 64;
 if(pi2_dc_val_v != NULL)
 {
        pi2_coeff_data[0] = *pi2_dc_val_v++;
        pi2_coeff_data[1 * 16] = *pi2_dc_val_v++;
        pi2_coeff_data[2 * 16] = *pi2_dc_val_v++;
        pi2_coeff_data[3 * 16] = *pi2_dc_val_v++;
 }
 else
 {
        pi2_coeff_data[0] = 0;
        pi2_coeff_data[1 * 16] = 0;
        pi2_coeff_data[2 * 16] = 0;
        pi2_coeff_data[3 * 16] = 0;
 }
}
