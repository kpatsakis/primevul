static void free_tables(H264Context *h, int free_rbsp)
{
    int i;
    H264Context *hx;

    av_freep(&h->intra4x4_pred_mode);
    av_freep(&h->chroma_pred_mode_table);
    av_freep(&h->cbp_table);
    av_freep(&h->mvd_table[0]);
    av_freep(&h->mvd_table[1]);
    av_freep(&h->direct_table);
    av_freep(&h->non_zero_count);
    av_freep(&h->slice_table_base);
    h->slice_table = NULL;
    av_freep(&h->list_counts);

    av_freep(&h->mb2b_xy);
    av_freep(&h->mb2br_xy);

    for (i = 0; i < 3; i++)
        av_freep(&h->visualization_buffer[i]);

    av_buffer_pool_uninit(&h->qscale_table_pool);
    av_buffer_pool_uninit(&h->mb_type_pool);
    av_buffer_pool_uninit(&h->motion_val_pool);
    av_buffer_pool_uninit(&h->ref_index_pool);

    if (free_rbsp && h->DPB) {
        for (i = 0; i < MAX_PICTURE_COUNT; i++)
            unref_picture(h, &h->DPB[i]);
        av_freep(&h->DPB);
    } else if (h->DPB) {
        for (i = 0; i < MAX_PICTURE_COUNT; i++)
            h->DPB[i].needs_realloc = 1;
    }

    h->cur_pic_ptr = NULL;

    for (i = 0; i < MAX_THREADS; i++) {
        hx = h->thread_context[i];
        if (!hx)
            continue;
        av_freep(&hx->top_borders[1]);
        av_freep(&hx->top_borders[0]);
        av_freep(&hx->bipred_scratchpad);
        av_freep(&hx->edge_emu_buffer);
        av_freep(&hx->dc_val_base);
        av_freep(&hx->me.scratchpad);
        av_freep(&hx->er.mb_index2xy);
        av_freep(&hx->er.error_status_table);
        av_freep(&hx->er.er_temp_buffer);
        av_freep(&hx->er.mbintra_table);
        av_freep(&hx->er.mbskip_table);

        if (free_rbsp) {
            av_freep(&hx->rbsp_buffer[1]);
            av_freep(&hx->rbsp_buffer[0]);
            hx->rbsp_buffer_size[0] = 0;
            hx->rbsp_buffer_size[1] = 0;
        }
        if (i)
            av_freep(&h->thread_context[i]);
    }
}
