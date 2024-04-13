bool V4L2JpegEncodeAccelerator::EncodedInstance::EnqueueInputRecord() {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  DCHECK(!input_job_queue_.empty());
  DCHECK(!free_input_buffers_.empty());

  std::unique_ptr<JobRecord> job_record = std::move(input_job_queue_.front());
  input_job_queue_.pop();
  const int index = free_input_buffers_.back();
  I420BufferRecord& input_record = input_buffer_map_[index];
  DCHECK(!input_record.at_device);

  uint8_t* src_y = job_record->input_frame->data(VideoFrame::kYPlane);
  uint8_t* src_u = job_record->input_frame->data(VideoFrame::kUPlane);
  uint8_t* src_v = job_record->input_frame->data(VideoFrame::kVPlane);
  size_t src_y_stride = job_record->input_frame->stride(VideoFrame::kYPlane);
  size_t src_u_stride = job_record->input_frame->stride(VideoFrame::kUPlane);
  size_t src_v_stride = job_record->input_frame->stride(VideoFrame::kVPlane);

  size_t input_coded_width = job_record->input_frame->coded_size().width();
  size_t input_coded_height = job_record->input_frame->coded_size().height();

  size_t dst_y_stride = bytes_per_line_[0];
  size_t dst_u_stride;
  size_t dst_v_stride;

  uint8_t* dst_y = static_cast<uint8_t*>(input_record.address[0]);
  uint8_t* dst_u;
  uint8_t* dst_v;

  if (input_buffer_num_planes_ == 1) {
    dst_u_stride = dst_y_stride / 2;
    dst_v_stride = dst_y_stride / 2;
    dst_u = dst_y + dst_y_stride * input_buffer_height_;
    dst_v = dst_u + dst_u_stride * input_buffer_height_ / 2;
  } else {
    DCHECK(input_buffer_num_planes_ == 3);

    dst_u_stride = bytes_per_line_[1];
    dst_v_stride = bytes_per_line_[2];

    dst_u = static_cast<uint8_t*>(input_record.address[1]);
    dst_v = static_cast<uint8_t*>(input_record.address[2]);
  }

  if (libyuv::I420Copy(src_y, src_y_stride, src_u, src_u_stride, src_v,
                       src_v_stride, dst_y, dst_y_stride, dst_u, dst_u_stride,
                       dst_v, dst_v_stride, input_coded_width,
                       input_coded_height)) {
    VLOGF(1) << "I420Copy failed";
    return false;
  }

  struct v4l2_buffer qbuf;
  struct v4l2_plane planes[kMaxI420Plane];
  memset(&qbuf, 0, sizeof(qbuf));
  memset(planes, 0, sizeof(planes));
  qbuf.index = index;
  qbuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
  qbuf.memory = V4L2_MEMORY_MMAP;
  qbuf.length = base::size(planes);
  for (size_t i = 0; i < input_buffer_num_planes_; i++) {
    planes[i].bytesused = 0;
  }
  qbuf.m.planes = planes;
  IOCTL_OR_ERROR_RETURN_FALSE(VIDIOC_QBUF, &qbuf);
  input_record.at_device = true;
  running_job_queue_.push(std::move(job_record));
  free_input_buffers_.pop_back();

  DVLOGF(3) << "enqueued frame id=" << job_record->task_id << " to device.";
  return true;
}
