WORD32 ih264d_get_frame_dimensions(iv_obj_t *dec_hdl,
 void *pv_api_ip,
 void *pv_api_op)
{
 ih264d_ctl_get_frame_dimensions_ip_t *ps_ip;
 ih264d_ctl_get_frame_dimensions_op_t *ps_op;
 dec_struct_t *ps_dec = dec_hdl->pv_codec_handle;
    UWORD32 disp_wd, disp_ht, buffer_wd, buffer_ht, x_offset, y_offset;

    ps_ip = (ih264d_ctl_get_frame_dimensions_ip_t *)pv_api_ip;

    ps_op = (ih264d_ctl_get_frame_dimensions_op_t *)pv_api_op;
    UNUSED(ps_ip);
 if((NULL != ps_dec->ps_cur_sps) && (1 == (ps_dec->ps_cur_sps->u1_is_valid)))
 {
        disp_wd = ps_dec->u2_disp_width;
        disp_ht = ps_dec->u2_disp_height;

 if(0 == ps_dec->u4_share_disp_buf)
 {
            buffer_wd = disp_wd;
            buffer_ht = disp_ht;
 }
 else
 {
            buffer_wd = ps_dec->u2_frm_wd_y;
            buffer_ht = ps_dec->u2_frm_ht_y;
 }
 }
 else
 {
        disp_wd = 0;
        disp_ht = 0;

 if(0 == ps_dec->u4_share_disp_buf)
 {
            buffer_wd = disp_wd;
            buffer_ht = disp_ht;
 }
 else
 {
            buffer_wd = ALIGN16(disp_wd) + (PAD_LEN_Y_H << 1);
            buffer_ht = ALIGN16(disp_ht) + (PAD_LEN_Y_V << 2);
 }
 }
 if(ps_dec->u4_app_disp_width > buffer_wd)
        buffer_wd = ps_dec->u4_app_disp_width;

 if(0 == ps_dec->u4_share_disp_buf)
 {
        x_offset = 0;
        y_offset = 0;
 }
 else
 {
        y_offset = (PAD_LEN_Y_V << 1);
        x_offset = PAD_LEN_Y_H;

 if((NULL != ps_dec->ps_sps) && (1 == (ps_dec->ps_sps->u1_is_valid))
 && (0 != ps_dec->u2_crop_offset_y))
 {
            y_offset += ps_dec->u2_crop_offset_y / ps_dec->u2_frm_wd_y;
            x_offset += ps_dec->u2_crop_offset_y % ps_dec->u2_frm_wd_y;
 }
 }

    ps_op->u4_disp_wd[0] = disp_wd;
    ps_op->u4_disp_ht[0] = disp_ht;
    ps_op->u4_buffer_wd[0] = buffer_wd;
    ps_op->u4_buffer_ht[0] = buffer_ht;
    ps_op->u4_x_offset[0] = x_offset;
    ps_op->u4_y_offset[0] = y_offset;

    ps_op->u4_disp_wd[1] = ps_op->u4_disp_wd[2] = ((ps_op->u4_disp_wd[0] + 1)
 >> 1);
    ps_op->u4_disp_ht[1] = ps_op->u4_disp_ht[2] = ((ps_op->u4_disp_ht[0] + 1)
 >> 1);
    ps_op->u4_buffer_wd[1] = ps_op->u4_buffer_wd[2] = (ps_op->u4_buffer_wd[0]
 >> 1);
    ps_op->u4_buffer_ht[1] = ps_op->u4_buffer_ht[2] = (ps_op->u4_buffer_ht[0]
 >> 1);
    ps_op->u4_x_offset[1] = ps_op->u4_x_offset[2] =
 (ps_op->u4_x_offset[0] >> 1);
    ps_op->u4_y_offset[1] = ps_op->u4_y_offset[2] =
 (ps_op->u4_y_offset[0] >> 1);

 if((ps_dec->u1_chroma_format == IV_YUV_420SP_UV)
 || (ps_dec->u1_chroma_format == IV_YUV_420SP_VU))
 {
        ps_op->u4_disp_wd[2] = 0;
        ps_op->u4_disp_ht[2] = 0;
        ps_op->u4_buffer_wd[2] = 0;
        ps_op->u4_buffer_ht[2] = 0;
        ps_op->u4_x_offset[2] = 0;
        ps_op->u4_y_offset[2] = 0;

        ps_op->u4_disp_wd[1] <<= 1;
        ps_op->u4_buffer_wd[1] <<= 1;
        ps_op->u4_x_offset[1] <<= 1;
 }

 return IV_SUCCESS;

}
