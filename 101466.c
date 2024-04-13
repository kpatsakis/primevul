bool V4L2JpegEncodeAccelerator::EncodedInstanceDmaBuf::CreateBuffers(
    gfx::Size coded_size,
    const VideoFrameLayout& input_layout,
    size_t output_buffer_size) {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());

  if (!SetOutputBufferFormat(coded_size, output_buffer_size)) {
    return false;
  }

  if (!SetInputBufferFormat(coded_size, input_layout)) {
    return false;
  }

  if (!RequestInputBuffers()) {
    return false;
  }

  if (!RequestOutputBuffers()) {
    return false;
  }

  return true;
}
