static void ihevcd_fill_outargs(codec_t *ps_codec,
 ivd_video_decode_ip_t *ps_dec_ip,
 ivd_video_decode_op_t *ps_dec_op)
{

    ps_dec_op->u4_error_code = ihevcd_map_error((IHEVCD_ERROR_T)ps_codec->i4_error_code);
    ps_dec_op->u4_num_bytes_consumed = ps_dec_ip->u4_num_Bytes
 - ps_codec->i4_bytes_remaining;
 if(ps_codec->i4_sps_done)
 {
        ps_dec_op->u4_pic_wd = ps_codec->i4_disp_wd;
        ps_dec_op->u4_pic_ht = ps_codec->i4_disp_ht;
 }
 else
 {
        ps_dec_op->u4_pic_wd = 0;
        ps_dec_op->u4_pic_ht = 0;
 }

    ps_dec_op->e_pic_type = ps_codec->e_dec_pic_type;
    ps_dec_op->u4_frame_decoded_flag = ps_codec->i4_pic_present;
    ps_dec_op->u4_new_seq = 0;

    ps_dec_op->u4_output_present = 0;
    ps_dec_op->u4_progressive_frame_flag = 1;
 if(ps_codec->i4_sps_done)
 {
 sps_t *ps_sps = (ps_codec->s_parse.ps_sps_base + ps_codec->i4_sps_id);
 profile_tier_lvl_info_t *ps_ptl;
        ps_ptl = &ps_sps->s_ptl;
 if((0 == ps_ptl->s_ptl_gen.i1_general_progressive_source_flag) &&
 (1 == ps_ptl->s_ptl_gen.i1_general_interlaced_source_flag))
 {
            ps_dec_op->u4_progressive_frame_flag = 0;
 }
 }

    ps_dec_op->u4_is_ref_flag = 1;
    ps_dec_op->e_output_format = ps_codec->e_chroma_fmt;
    ps_dec_op->u4_is_ref_flag = 1;

    ps_dec_op->e4_fld_type = IV_FLD_TYPE_DEFAULT;

    ps_dec_op->u4_ts = (UWORD32)(-1);
    ps_dec_op->u4_disp_buf_id = ps_codec->i4_disp_buf_id;
 if(ps_codec->i4_flush_mode)
 {
        ps_dec_op->u4_num_bytes_consumed = 0;
 /*In the case of flush ,since no frame is decoded set pic type as invalid*/
        ps_dec_op->u4_is_ref_flag = 0;
        ps_dec_op->e_pic_type = IV_NA_FRAME;
        ps_dec_op->u4_frame_decoded_flag = 0;

 }
 /* If there is a display buffer */
 if(ps_codec->ps_disp_buf)
 {
 pic_buf_t *ps_disp_buf = ps_codec->ps_disp_buf;
 sei_params_t *ps_sei = &ps_disp_buf->s_sei_params;

 if(ps_sei->i1_sei_parameters_present_flag &&
           ps_sei->i1_pic_timing_params_present_flag)
 {
            UWORD32 u4_pic_struct;
            u4_pic_struct = ps_sei->s_pic_timing_sei_params.u4_pic_struct;
 switch(u4_pic_struct)
 {
 case 1:
                    ps_dec_op->e4_fld_type = IV_TOP_FLD;
                    ps_dec_op->u4_progressive_frame_flag = 0;
 break;
 case 2:
                    ps_dec_op->e4_fld_type = IV_BOT_FLD;
                    ps_dec_op->u4_progressive_frame_flag = 0;
 break;
 case 0:
 default:
                    ps_dec_op->e4_fld_type = IV_FLD_TYPE_DEFAULT;
                    ps_dec_op->u4_progressive_frame_flag = 1;
 break;
 }
 }
        ps_dec_op->u4_output_present = 1;
        ps_dec_op->u4_ts = ps_disp_buf->u4_ts;
 if((ps_codec->i4_flush_mode == 0) && (ps_codec->s_parse.i4_end_of_frame == 0))
            ps_dec_op->u4_output_present = 0;
        ps_dec_op->s_disp_frm_buf.u4_y_wd = ps_codec->i4_disp_wd;
        ps_dec_op->s_disp_frm_buf.u4_y_ht = ps_codec->i4_disp_ht;

 if(ps_codec->i4_share_disp_buf)
 {
            ps_dec_op->s_disp_frm_buf.pv_y_buf = ps_disp_buf->pu1_luma;
 if(ps_codec->e_chroma_fmt != IV_YUV_420P)
 {
                ps_dec_op->s_disp_frm_buf.pv_u_buf = ps_disp_buf->pu1_chroma;
                ps_dec_op->s_disp_frm_buf.pv_v_buf = NULL;
 }
 else
 {
                WORD32 i;
                UWORD8 *pu1_u_dst = NULL, *pu1_v_dst = NULL;
 for(i = 0; i < ps_codec->i4_share_disp_buf_cnt; i++)
 {
                    WORD32 diff = ps_disp_buf->pu1_luma - ps_codec->s_disp_buffer[i].pu1_bufs[0];
 if(diff == (ps_codec->i4_strd * PAD_TOP + PAD_LEFT))
 {
                        pu1_u_dst = ps_codec->s_disp_buffer[i].pu1_bufs[1];
                        pu1_u_dst += (ps_codec->i4_strd * PAD_TOP) / 4 + (PAD_LEFT / 2);

                        pu1_v_dst = ps_codec->s_disp_buffer[i].pu1_bufs[2];
                        pu1_v_dst += (ps_codec->i4_strd * PAD_TOP) / 4 + (PAD_LEFT / 2);
 break;
 }
 }
                ps_dec_op->s_disp_frm_buf.pv_u_buf = pu1_u_dst;
                ps_dec_op->s_disp_frm_buf.pv_v_buf = pu1_v_dst;
 }
            ps_dec_op->s_disp_frm_buf.u4_y_strd = ps_codec->i4_strd;
 }
 else
 {
            ps_dec_op->s_disp_frm_buf.pv_y_buf =
                            ps_dec_ip->s_out_buffer.pu1_bufs[0];
            ps_dec_op->s_disp_frm_buf.pv_u_buf =
                            ps_dec_ip->s_out_buffer.pu1_bufs[1];
            ps_dec_op->s_disp_frm_buf.pv_v_buf =
                            ps_dec_ip->s_out_buffer.pu1_bufs[2];
            ps_dec_op->s_disp_frm_buf.u4_y_strd = ps_codec->i4_disp_strd;
 }

 if((IV_YUV_420SP_VU == ps_codec->e_chroma_fmt)
 || (IV_YUV_420SP_UV == ps_codec->e_chroma_fmt))
 {
            ps_dec_op->s_disp_frm_buf.u4_u_strd =
                            ps_dec_op->s_disp_frm_buf.u4_y_strd;
            ps_dec_op->s_disp_frm_buf.u4_v_strd = 0;
            ps_dec_op->s_disp_frm_buf.u4_u_wd =
                            ps_dec_op->s_disp_frm_buf.u4_y_wd;
            ps_dec_op->s_disp_frm_buf.u4_v_wd = 0;
            ps_dec_op->s_disp_frm_buf.u4_u_ht =
                            ps_dec_op->s_disp_frm_buf.u4_y_ht / 2;
            ps_dec_op->s_disp_frm_buf.u4_v_ht = 0;
 }
 else if(IV_YUV_420P == ps_codec->e_chroma_fmt)
 {
            ps_dec_op->s_disp_frm_buf.u4_u_strd =
                            ps_dec_op->s_disp_frm_buf.u4_y_strd / 2;
            ps_dec_op->s_disp_frm_buf.u4_v_strd =
                            ps_dec_op->s_disp_frm_buf.u4_y_strd / 2;
            ps_dec_op->s_disp_frm_buf.u4_u_wd =
                            ps_dec_op->s_disp_frm_buf.u4_y_wd / 2;
            ps_dec_op->s_disp_frm_buf.u4_v_wd =
                            ps_dec_op->s_disp_frm_buf.u4_y_wd / 2;
            ps_dec_op->s_disp_frm_buf.u4_u_ht =
                            ps_dec_op->s_disp_frm_buf.u4_y_ht / 2;
            ps_dec_op->s_disp_frm_buf.u4_v_ht =
                            ps_dec_op->s_disp_frm_buf.u4_y_ht / 2;
 }

 }
 else if(ps_codec->i4_flush_mode)
 {
        ps_dec_op->u4_error_code = IHEVCD_END_OF_SEQUENCE;
 /* Come out of flush mode */
        ps_codec->i4_flush_mode = 0;
 }

}
