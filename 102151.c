static vpx_codec_err_t decoder_destroy(vpx_codec_alg_priv_t *ctx) {
 if (ctx->frame_workers != NULL) {
 int i;
 for (i = 0; i < ctx->num_frame_workers; ++i) {
 VPxWorker *const worker = &ctx->frame_workers[i];
 FrameWorkerData *const frame_worker_data =
 (FrameWorkerData *)worker->data1;
      vpx_get_worker_interface()->end(worker);
      vp9_remove_common(&frame_worker_data->pbi->common);
#if CONFIG_VP9_POSTPROC
      vp9_free_postproc_buffers(&frame_worker_data->pbi->common);
#endif
      vp9_decoder_remove(frame_worker_data->pbi);
      vpx_free(frame_worker_data->scratch_buffer);
#if CONFIG_MULTITHREAD
      pthread_mutex_destroy(&frame_worker_data->stats_mutex);
      pthread_cond_destroy(&frame_worker_data->stats_cond);
#endif
      vpx_free(frame_worker_data);
 }
#if CONFIG_MULTITHREAD
    pthread_mutex_destroy(&ctx->buffer_pool->pool_mutex);
#endif
 }

 if (ctx->buffer_pool) {
    vp9_free_ref_frame_buffers(ctx->buffer_pool);
    vp9_free_internal_frame_buffers(&ctx->buffer_pool->int_frame_buffers);
 }

  vpx_free(ctx->frame_workers);
  vpx_free(ctx->buffer_pool);
  vpx_free(ctx);
 return VPX_CODEC_OK;
}
