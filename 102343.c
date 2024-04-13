WORD32 ih264d_create_mv_bank(void *pv_dec,
                             UWORD32 ui_width,
                             UWORD32 ui_height)
{
    UWORD8  i;
    UWORD32 col_flag_buffer_size, mvpred_buffer_size;
    UWORD8 *pu1_mv_buf_mgr_base, *pu1_mv_bank_base;
 col_mv_buf_t *ps_col_mv;
 mv_pred_t *ps_mv;
    UWORD8 *pu1_col_zero_flag_buf;
 dec_struct_t *ps_dec = (dec_struct_t *)pv_dec;
    WORD32 buf_ret;
    UWORD32 u4_num_bufs;
    UWORD8 *pu1_buf;
    WORD32 size;
 void *pv_mem_ctxt = ps_dec->pv_mem_ctxt;

    col_flag_buffer_size = ((ui_width * ui_height) >> 4);
    mvpred_buffer_size = sizeof(mv_pred_t)
 * ((ui_width * (ui_height + PAD_MV_BANK_ROW)) >> 4);

    ih264_buf_mgr_init((buf_mgr_t *)ps_dec->pv_mv_buf_mgr);

    ps_col_mv = ps_dec->ps_col_mv_base;

    u4_num_bufs = ps_dec->ps_cur_sps->u1_num_ref_frames + 1;

    u4_num_bufs = MIN(u4_num_bufs, ps_dec->u1_pic_bufs);
    u4_num_bufs = MAX(u4_num_bufs, 2);
    pu1_buf = ps_dec->pu1_mv_bank_buf_base;
 for(i = 0 ; i < u4_num_bufs ; i++)
 {
        pu1_col_zero_flag_buf = pu1_buf;
        pu1_buf += ALIGN64(col_flag_buffer_size);

        ps_mv = (mv_pred_t *)pu1_buf;
        pu1_buf += ALIGN64(mvpred_buffer_size);

        memset(ps_mv, 0, ((ui_width * OFFSET_MV_BANK_ROW) >> 4) * sizeof(mv_pred_t));
        ps_mv += (ui_width*OFFSET_MV_BANK_ROW) >> 4;

        ps_col_mv->pv_col_zero_flag = (void *)pu1_col_zero_flag_buf;
        ps_col_mv->pv_mv = (void *)ps_mv;
        buf_ret = ih264_buf_mgr_add((buf_mgr_t *)ps_dec->pv_mv_buf_mgr, ps_col_mv, i);
 if(0 != buf_ret)
 {
            ps_dec->i4_error_code = ERROR_BUF_MGR;
 return ERROR_BUF_MGR;
 }
        ps_col_mv++;
 }
 return OK;
}
