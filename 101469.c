bool V4L2JpegEncodeAccelerator::EncodedInstanceDmaBuf::EnqueueOutputRecord() {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  DCHECK(!free_output_buffers_.empty());

  const int index = free_output_buffers_.back();
  struct v4l2_buffer qbuf;
  struct v4l2_plane planes[kMaxJpegPlane];
  memset(&qbuf, 0, sizeof(qbuf));
  memset(planes, 0, sizeof(planes));
  qbuf.index = index;
  qbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  qbuf.memory = V4L2_MEMORY_DMABUF;
  qbuf.length = base::size(planes);
  qbuf.m.planes = planes;

  auto& job_record = running_job_queue_.back();
  for (size_t i = 0; i < qbuf.length; i++) {
    planes[i].m.fd = job_record->output_frame->DmabufFds()[i].get();
  }
  IOCTL_OR_ERROR_RETURN_FALSE(VIDIOC_QBUF, &qbuf);
  free_output_buffers_.pop_back();
  return true;
}
