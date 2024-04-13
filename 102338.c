WORD32 ihevcd_calc_poc(codec_t *ps_codec, nal_header_t *ps_nal, WORD8 i1_log2_max_poc_lsb, WORD32 i2_poc_lsb)
{
    WORD32 i4_abs_poc, i4_poc_msb;
    WORD32 max_poc_lsb;
    WORD8 i1_nal_unit_type = ps_nal->i1_nal_unit_type;
    max_poc_lsb = (1 << i1_log2_max_poc_lsb);

 if((!ps_codec->i4_first_pic_done) && (!ps_codec->i4_pic_present))
        ps_codec->i4_prev_poc_msb = -2 * max_poc_lsb;

 if(NAL_IDR_N_LP == i1_nal_unit_type
 || NAL_IDR_W_LP == i1_nal_unit_type
 || NAL_BLA_N_LP == i1_nal_unit_type
 || NAL_BLA_W_DLP == i1_nal_unit_type
 || NAL_BLA_W_LP == i1_nal_unit_type
 || (NAL_CRA == i1_nal_unit_type && !ps_codec->i4_first_pic_done))
 {
        i4_poc_msb = ps_codec->i4_prev_poc_msb + 2 * max_poc_lsb;
        ps_codec->i4_prev_poc_lsb = 0;
        ps_codec->i4_max_prev_poc_lsb = 0;
 }
 else
 {

 if((i2_poc_lsb < ps_codec->i4_prev_poc_lsb)
 && ((ps_codec->i4_prev_poc_lsb - i2_poc_lsb) >= max_poc_lsb / 2))
 {
            i4_poc_msb = ps_codec->i4_prev_poc_msb + max_poc_lsb;
 }
 else if((i2_poc_lsb > ps_codec->i4_prev_poc_lsb)
 && ((i2_poc_lsb - ps_codec->i4_prev_poc_lsb) > max_poc_lsb / 2))
 {
            i4_poc_msb = ps_codec->i4_prev_poc_msb - max_poc_lsb;
 }
 else
 {
            i4_poc_msb = ps_codec->i4_prev_poc_msb;
 }


 }

    i4_abs_poc = i4_poc_msb + i2_poc_lsb;
    ps_codec->i4_max_prev_poc_lsb = MAX(ps_codec->i4_max_prev_poc_lsb, i2_poc_lsb);

 {
        WORD32 is_reference_nal = ((i1_nal_unit_type <= NAL_RSV_VCL_R15) && (i1_nal_unit_type % 2 != 0)) || ((i1_nal_unit_type >= NAL_BLA_W_LP) && (i1_nal_unit_type <= NAL_RSV_RAP_VCL23));
        WORD32 update_prev_poc = ((is_reference_nal) && ((i1_nal_unit_type < NAL_RADL_N) || (i1_nal_unit_type > NAL_RASL_R)));

 if((0 == ps_nal->i1_nuh_temporal_id) &&
 (update_prev_poc))
 {
            ps_codec->i4_prev_poc_lsb = i2_poc_lsb;
            ps_codec->i4_prev_poc_msb = i4_poc_msb;
 }
 }

 return i4_abs_poc;
}
