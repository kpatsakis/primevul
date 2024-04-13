static vpx_codec_err_t ctrl_get_bit_depth(vpx_codec_alg_priv_t *ctx,
                                          va_list args) {
 unsigned int *const bit_depth = va_arg(args, unsigned int *);
 VPxWorker *const worker = &ctx->frame_workers[ctx->next_output_worker_id];

 if (bit_depth) {
 if (worker) {
 FrameWorkerData *const frame_worker_data =
 (FrameWorkerData *)worker->data1;
 const VP9_COMMON *const cm = &frame_worker_data->pbi->common;
 *bit_depth = cm->bit_depth;
 return VPX_CODEC_OK;
 } else {
 return VPX_CODEC_ERROR;
 }
 }

 return VPX_CODEC_INVALID_PARAM;
}
