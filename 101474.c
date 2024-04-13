bool V4L2JpegEncodeAccelerator::EncodedInstance::RequestOutputBuffers() {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  struct v4l2_requestbuffers reqbufs;
  memset(&reqbufs, 0, sizeof(reqbufs));
  reqbufs.count = kBufferCount;
  reqbufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  reqbufs.memory = V4L2_MEMORY_MMAP;
  IOCTL_OR_ERROR_RETURN_FALSE(VIDIOC_REQBUFS, &reqbufs);

  DCHECK(output_buffer_map_.empty());
  output_buffer_map_.resize(reqbufs.count);

  for (size_t i = 0; i < output_buffer_map_.size(); ++i) {
    free_output_buffers_.push_back(i);

    struct v4l2_buffer buffer;
    struct v4l2_plane planes[kMaxJpegPlane];
    memset(&buffer, 0, sizeof(buffer));
    memset(planes, 0, sizeof(planes));
    buffer.index = i;
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    buffer.m.planes = planes;
    buffer.length = base::size(planes);
    buffer.memory = V4L2_MEMORY_MMAP;
    IOCTL_OR_ERROR_RETURN_FALSE(VIDIOC_QUERYBUF, &buffer);
    if (buffer.length != kMaxJpegPlane) {
      return false;
    }
    void* address =
        device_->Mmap(NULL, planes[0].length, PROT_READ | PROT_WRITE,
                      MAP_SHARED, planes[0].m.mem_offset);
    if (address == MAP_FAILED) {
      VPLOGF(1) << "mmap() failed";
      return false;
    }
    output_buffer_map_[i].address[0] = address;
    output_buffer_map_[i].length[0] = planes[0].length;
  }

  return true;
}
