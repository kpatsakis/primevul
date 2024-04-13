WORD32 ih264d_allocate_static_bufs(iv_obj_t **dec_hdl, void *pv_api_ip, void *pv_api_op)
{
 ih264d_create_ip_t *ps_create_ip;
 ih264d_create_op_t *ps_create_op;
 void *pv_buf;
    UWORD8 *pu1_buf;
 dec_struct_t *ps_dec;
 void *(*pf_aligned_alloc)(void *pv_mem_ctxt, WORD32 alignment, WORD32 size);
 void (*pf_aligned_free)(void *pv_mem_ctxt, void *pv_buf);
 void *pv_mem_ctxt;
    WORD32 size;

    ps_create_ip = (ih264d_create_ip_t *)pv_api_ip;
    ps_create_op = (ih264d_create_op_t *)pv_api_op;

    ps_create_op->s_ivd_create_op_t.u4_error_code = 0;

    pf_aligned_alloc = ps_create_ip->s_ivd_create_ip_t.pf_aligned_alloc;
    pf_aligned_free = ps_create_ip->s_ivd_create_ip_t.pf_aligned_free;
    pv_mem_ctxt  = ps_create_ip->s_ivd_create_ip_t.pv_mem_ctxt;

 /* Initialize return handle to NULL */
    ps_create_op->s_ivd_create_op_t.pv_handle = NULL;
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, sizeof(iv_obj_t));
    RETURN_IF((NULL == pv_buf), IV_FAIL);
 *dec_hdl = (iv_obj_t *)pv_buf;
    ps_create_op->s_ivd_create_op_t.pv_handle = *dec_hdl;

 (*dec_hdl)->pv_codec_handle = NULL;
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, sizeof(dec_struct_t));
    RETURN_IF((NULL == pv_buf), IV_FAIL);
 (*dec_hdl)->pv_codec_handle = (dec_struct_t *)pv_buf;
    ps_dec = (dec_struct_t *)pv_buf;

    memset(ps_dec, 0, sizeof(dec_struct_t));

#ifndef LOGO_EN
    ps_dec->u4_share_disp_buf = ps_create_ip->s_ivd_create_ip_t.u4_share_disp_buf;
#else
    ps_dec->u4_share_disp_buf = 0;
#endif

    ps_dec->u1_chroma_format =
 (UWORD8)(ps_create_ip->s_ivd_create_ip_t.e_output_format);

 if((ps_dec->u1_chroma_format != IV_YUV_420P)
 && (ps_dec->u1_chroma_format
 != IV_YUV_420SP_UV)
 && (ps_dec->u1_chroma_format
 != IV_YUV_420SP_VU))
 {
        ps_dec->u4_share_disp_buf = 0;
 }

    ps_dec->pf_aligned_alloc = pf_aligned_alloc;
    ps_dec->pf_aligned_free = pf_aligned_free;
    ps_dec->pv_mem_ctxt = pv_mem_ctxt;


    size = ((sizeof(dec_seq_params_t)) * MAX_NUM_SEQ_PARAMS);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_sps = pv_buf;

    size = (sizeof(dec_pic_params_t)) * MAX_NUM_PIC_PARAMS;
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_pps = pv_buf;

    size = ithread_get_handle_size();
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pv_dec_thread_handle = pv_buf;

    size = ithread_get_handle_size();
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pv_bs_deblk_thread_handle = pv_buf;

    size = sizeof(dpb_manager_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_dpb_mgr = pv_buf;

    size = sizeof(pred_info_t) * 2 * 32;
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_pred = pv_buf;

    size = sizeof(disp_mgr_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pv_disp_buf_mgr = pv_buf;

    size = sizeof(buf_mgr_t) + ithread_get_mutex_lock_size();
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pv_pic_buf_mgr = pv_buf;

    size = sizeof(struct pic_buffer_t) * (H264_MAX_REF_PICS * 2);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_pic_buf_base = pv_buf;

    size = sizeof(dec_err_status_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_dec_err_status = (dec_err_status_t *)pv_buf;

    size = sizeof(sei);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_sei = (sei *)pv_buf;

    size = sizeof(dpb_commands_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_dpb_cmds = (dpb_commands_t *)pv_buf;

    size = sizeof(dec_bit_stream_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_bitstrm = (dec_bit_stream_t *)pv_buf;

    size = sizeof(dec_slice_params_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_cur_slice = (dec_slice_params_t *)pv_buf;

    size = MAX(sizeof(dec_seq_params_t), sizeof(dec_pic_params_t));
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pv_scratch_sps_pps = pv_buf;


    ps_dec->u4_static_bits_buf_size = 256000;
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, ps_dec->u4_static_bits_buf_size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pu1_bits_buf_static = pv_buf;


    size = ((TOTAL_LIST_ENTRIES + PAD_MAP_IDX_POC)
 * sizeof(void *));
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ppv_map_ref_idx_to_poc_base = pv_buf;
    memset(ps_dec->ppv_map_ref_idx_to_poc_base, 0, size);

    ps_dec->ppv_map_ref_idx_to_poc = ps_dec->ppv_map_ref_idx_to_poc_base + OFFSET_MAP_IDX_POC;


    size = (sizeof(bin_ctxt_model_t) * NUM_CABAC_CTXTS);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->p_cabac_ctxt_table_t = pv_buf;



    size = sizeof(ctxt_inc_mb_info_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_left_mb_ctxt_info = pv_buf;



    size = MAX_REF_BUF_SIZE * 2;
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pu1_ref_buff_base = pv_buf;
    ps_dec->pu1_ref_buff = ps_dec->pu1_ref_buff_base + MAX_REF_BUF_SIZE;


    size = ((sizeof(WORD16)) * PRED_BUFFER_WIDTH
 * PRED_BUFFER_HEIGHT * 2);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pi2_pred1 = pv_buf;


    size = sizeof(UWORD8) * (MB_LUM_SIZE);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pu1_temp_mc_buffer = pv_buf;




    size = 8 * MAX_REF_BUFS * sizeof(struct pic_buffer_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);

    ps_dec->pu1_init_dpb_base = pv_buf;
    pu1_buf = pv_buf;
    ps_dec->ps_dpb_mgr->ps_init_dpb[0][0] = (struct pic_buffer_t *)pu1_buf;

    pu1_buf += size / 2;
    ps_dec->ps_dpb_mgr->ps_init_dpb[1][0] = (struct pic_buffer_t *)pu1_buf;

    size = (sizeof(UWORD32) * 3
 * (MAX_FRAMES * MAX_FRAMES))
 << 3;
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pu4_mbaff_wt_mat = pv_buf;

    size = sizeof(UWORD32) * 2 * 3
 * (MAX_FRAMES * MAX_FRAMES);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pu4_wts_ofsts_mat = pv_buf;


    size = (sizeof(neighbouradd_t) << 2);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_left_mvpred_addr = pv_buf;


    size = sizeof(buf_mgr_t) + ithread_get_mutex_lock_size();
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->pv_mv_buf_mgr = pv_buf;


    size = sizeof(col_mv_buf_t) * (H264_MAX_REF_PICS * 2);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_dec->ps_col_mv_base = pv_buf;
    memset(ps_dec->ps_col_mv_base, 0, size);

 {
        UWORD8 i;
 struct pic_buffer_t *ps_init_dpb;
        ps_init_dpb = ps_dec->ps_dpb_mgr->ps_init_dpb[0][0];
 for(i = 0; i < 2 * MAX_REF_BUFS; i++)
 {
            ps_init_dpb->pu1_buf1 = NULL;
            ps_init_dpb->u1_long_term_frm_idx = MAX_REF_BUFS + 1;
            ps_dec->ps_dpb_mgr->ps_init_dpb[0][i] = ps_init_dpb;
            ps_dec->ps_dpb_mgr->ps_mod_dpb[0][i] = ps_init_dpb;
            ps_init_dpb++;
 }

        ps_init_dpb = ps_dec->ps_dpb_mgr->ps_init_dpb[1][0];
 for(i = 0; i < 2 * MAX_REF_BUFS; i++)
 {
            ps_init_dpb->pu1_buf1 = NULL;
            ps_init_dpb->u1_long_term_frm_idx = MAX_REF_BUFS + 1;
            ps_dec->ps_dpb_mgr->ps_init_dpb[1][i] = ps_init_dpb;
            ps_dec->ps_dpb_mgr->ps_mod_dpb[1][i] = ps_init_dpb;
            ps_init_dpb++;
 }
 }
    ih264d_init_decoder(ps_dec);

 return IV_SUCCESS;
}
