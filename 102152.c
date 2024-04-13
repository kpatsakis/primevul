static int frame_worker_hook(void *arg1, void *arg2) {
 FrameWorkerData *const frame_worker_data = (FrameWorkerData *)arg1;
 const uint8_t *data = frame_worker_data->data;
 (void)arg2;

  frame_worker_data->result =
      vp9_receive_compressed_data(frame_worker_data->pbi,
                                  frame_worker_data->data_size,
 &data);
  frame_worker_data->data_end = data;

 if (frame_worker_data->pbi->frame_parallel_decode) {
 if (frame_worker_data->result != 0 ||
        frame_worker_data->data + frame_worker_data->data_size - 1 > data) {
 VPxWorker *const worker = frame_worker_data->pbi->frame_worker_owner;
 BufferPool *const pool = frame_worker_data->pbi->common.buffer_pool;
      vp9_frameworker_lock_stats(worker);
      frame_worker_data->frame_context_ready = 1;
      lock_buffer_pool(pool);
      frame_worker_data->pbi->cur_buf->buf.corrupted = 1;
      unlock_buffer_pool(pool);
      frame_worker_data->pbi->need_resync = 1;
      vp9_frameworker_signal_stats(worker);
      vp9_frameworker_unlock_stats(worker);
 return 0;
 }
 } else if (frame_worker_data->result != 0) {
    frame_worker_data->pbi->cur_buf->buf.corrupted = 1;
    frame_worker_data->pbi->need_resync = 1;
 }
 return !frame_worker_data->result;
}
