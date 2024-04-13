void V4L2JpegEncodeAccelerator::EncodedInstanceDmaBuf::ServiceDevice() {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());

  if (!running_job_queue_.empty()) {
    Dequeue();
  }

  EnqueueInput();
  EnqueueOutput();

  DVLOGF(3) << "buffer counts: INPUT[" << input_job_queue_.size()
            << "] => DEVICE[" << free_input_buffers_.size() << "/"
            << "->" << free_output_buffers_.size() << "]";
}
