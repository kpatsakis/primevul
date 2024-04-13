void GetMetadataFromFrame(const media::VideoFrame& frame,
                          double* device_scale_factor,
                          double* page_scale_factor,
                          gfx::Vector2dF* root_scroll_offset,
                          double* top_controls_height,
                          double* top_controls_shown_ratio) {
  bool success = true;
  double root_scroll_offset_x, root_scroll_offset_y;
  success &= frame.metadata()->GetDouble(
      media::VideoFrameMetadata::DEVICE_SCALE_FACTOR, device_scale_factor);
  success &= frame.metadata()->GetDouble(
      media::VideoFrameMetadata::PAGE_SCALE_FACTOR, page_scale_factor);
  success &= frame.metadata()->GetDouble(
      media::VideoFrameMetadata::ROOT_SCROLL_OFFSET_X, &root_scroll_offset_x);
  success &= frame.metadata()->GetDouble(
      media::VideoFrameMetadata::ROOT_SCROLL_OFFSET_Y, &root_scroll_offset_y);
  success &= frame.metadata()->GetDouble(
      media::VideoFrameMetadata::TOP_CONTROLS_HEIGHT, top_controls_height);
  success &= frame.metadata()->GetDouble(
      media::VideoFrameMetadata::TOP_CONTROLS_SHOWN_RATIO,
      top_controls_shown_ratio);
  DCHECK(success);

  root_scroll_offset->set_x(root_scroll_offset_x);
  root_scroll_offset->set_y(root_scroll_offset_y);
}
