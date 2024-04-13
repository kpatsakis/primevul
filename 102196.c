UWORD32 ih264d_unpack_coeff4x4_8x8blk(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info,
                                   UWORD16 ui2_luma_csbp,
                                   WORD16 *pi2_out_coeff_data)
{
    UWORD8 *pu1_inv_scan;
    UWORD8 u1_mb_field_decoding_flag = ps_cur_mb_info->u1_mb_field_decodingflag;
    UWORD8 u1_field_coding_flag = ps_cur_mb_info->ps_curmb->u1_mb_fld;
    UWORD32 u4_luma_dc_only_csbp = 0;
    WORD32 dc_only_flag = 0;

    PROFILE_DISABLE_UNPACK_LUMA()
 if(u1_field_coding_flag || u1_mb_field_decoding_flag)
 {
        pu1_inv_scan = (UWORD8 *)gau1_ih264d_inv_scan_fld;
 }
 else
 {
        pu1_inv_scan = (UWORD8 *)gau1_ih264d_inv_scan;
 }

 if(ui2_luma_csbp & 0x1)
 {
        memset(pi2_out_coeff_data,0,16*sizeof(WORD16));
        dc_only_flag = ih264d_unpack_coeff4x4_4x4blk(ps_dec,
                                      pi2_out_coeff_data,
                                      pu1_inv_scan);

        INSERT_BIT(u4_luma_dc_only_csbp, 0, dc_only_flag);
 }

    pi2_out_coeff_data += 16;
 if(ui2_luma_csbp & 0x2)
 {
        memset(pi2_out_coeff_data,0,16*sizeof(WORD16));
        dc_only_flag = ih264d_unpack_coeff4x4_4x4blk(ps_dec,
                                      pi2_out_coeff_data,
                                      pu1_inv_scan);
        INSERT_BIT(u4_luma_dc_only_csbp, 1, dc_only_flag);
 }

    pi2_out_coeff_data += 16 + 32;
 if(ui2_luma_csbp & 0x10)
 {
        memset(pi2_out_coeff_data,0,16*sizeof(WORD16));
        dc_only_flag = ih264d_unpack_coeff4x4_4x4blk(ps_dec,
                                      pi2_out_coeff_data,
                                      pu1_inv_scan);
        INSERT_BIT(u4_luma_dc_only_csbp, 4, dc_only_flag);
 }

    pi2_out_coeff_data += 16;
 if(ui2_luma_csbp & 0x20)
 {
        memset(pi2_out_coeff_data,0,16*sizeof(WORD16));
        dc_only_flag = ih264d_unpack_coeff4x4_4x4blk(ps_dec,
                                      pi2_out_coeff_data,
                                      pu1_inv_scan);
        INSERT_BIT(u4_luma_dc_only_csbp, 5, dc_only_flag);
 }
 return u4_luma_dc_only_csbp;
}
