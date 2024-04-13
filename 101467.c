void V4L2JpegEncodeAccelerator::EncodedInstance::DestroyInputBuffers() {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  free_input_buffers_.clear();

  if (input_buffer_map_.empty())
    return;

  if (input_streamon_) {
    __u32 type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    IOCTL_OR_ERROR_RETURN(VIDIOC_STREAMOFF, &type);
    input_streamon_ = false;
  }

  for (const auto& input_record : input_buffer_map_) {
    for (size_t i = 0; i < input_buffer_num_planes_; ++i) {
      device_->Munmap(input_record.address[i], input_record.length[i]);
    }
  }

  struct v4l2_requestbuffers reqbufs;
  memset(&reqbufs, 0, sizeof(reqbufs));
  reqbufs.count = 0;
  reqbufs.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
  reqbufs.memory = V4L2_MEMORY_MMAP;
  IOCTL_OR_LOG_ERROR(VIDIOC_REQBUFS, &reqbufs);

  input_buffer_map_.clear();
  input_buffer_num_planes_ = 0;
}
