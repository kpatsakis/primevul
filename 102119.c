void vp9_init_context_buffers(VP9_COMMON *cm) {
  cm->setup_mi(cm);
 if (cm->last_frame_seg_map && !cm->frame_parallel_decode)
    memset(cm->last_frame_seg_map, 0, cm->mi_rows * cm->mi_cols);
}
