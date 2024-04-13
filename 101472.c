void V4L2JpegEncodeAccelerator::EncodedInstance::NotifyError(int32_t task_id,
                                                             Status status) {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  parent_->NotifyError(task_id, status);
}
