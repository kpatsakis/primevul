void ihevcd_copy_sps(codec_t *ps_codec, WORD32 sps_id, WORD32 sps_id_ref)
{
 sps_t *ps_sps, *ps_sps_ref;
    WORD16 *pi2_scaling_mat_backup;
    WORD32 scaling_mat_size;

    SCALING_MAT_SIZE(scaling_mat_size);
    ps_sps_ref = ps_codec->ps_sps_base + sps_id_ref;
    ps_sps = ps_codec->ps_sps_base + sps_id;

 if(ps_sps->i1_sps_valid)
 {
 if((ps_sps->i1_log2_ctb_size != ps_sps_ref->i1_log2_ctb_size) ||
 (ps_sps->i2_pic_wd_in_ctb != ps_sps_ref->i2_pic_wd_in_ctb) ||
 (ps_sps->i2_pic_ht_in_ctb != ps_sps_ref->i2_pic_ht_in_ctb))
 {
            ihevcd_unmark_pps(ps_codec, sps_id);
 }
 }

    pi2_scaling_mat_backup = ps_sps->pi2_scaling_mat;

    memcpy(ps_sps, ps_sps_ref, sizeof(sps_t));
    ps_sps->pi2_scaling_mat = pi2_scaling_mat_backup;
    memcpy(ps_sps->pi2_scaling_mat, ps_sps_ref->pi2_scaling_mat, scaling_mat_size * sizeof(WORD16));
    ps_sps->i1_sps_valid = 1;

    ps_codec->s_parse.ps_sps = ps_sps;
}
