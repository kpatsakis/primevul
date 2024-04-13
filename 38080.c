static int update_thread_context(AVCodecContext *dst, const AVCodecContext *src)
{
    FFV1Context *fsrc = src->priv_data;
    FFV1Context *fdst = dst->priv_data;
    int i, ret;

    if (dst == src)
        return 0;

    if (!fdst->picture.f) {
        memcpy(fdst, fsrc, sizeof(*fdst));

        for (i = 0; i < fdst->quant_table_count; i++) {
            fdst->initial_states[i] = av_malloc(fdst->context_count[i] * sizeof(*fdst->initial_states[i]));
            memcpy(fdst->initial_states[i], fsrc->initial_states[i], fdst->context_count[i] * sizeof(*fdst->initial_states[i]));
        }

        fdst->picture.f      = av_frame_alloc();
        fdst->last_picture.f = av_frame_alloc();

        if ((ret = ffv1_init_slice_contexts(fdst)) < 0)
            return ret;
    }

    av_assert1(fdst->slice_count == fsrc->slice_count);

    fdst->key_frame_ok = fsrc->key_frame_ok;

    ff_thread_release_buffer(dst, &fdst->picture);
    if (fsrc->picture.f->data[0]) {
        if ((ret = ff_thread_ref_frame(&fdst->picture, &fsrc->picture)) < 0)
            return ret;
    }
    for (i = 0; i < fdst->slice_count; i++) {
        FFV1Context *fsdst = fdst->slice_context[i];
        FFV1Context *fssrc = fsrc->slice_context[i];

        fsdst->slice_damaged = fssrc->slice_damaged;
    }

    fdst->fsrc = fsrc;

    return 0;
}
