void CommandBufferProxyImpl::SetGetBuffer(int32_t shm_id) {
  CheckLock();
  base::AutoLock lock(last_state_lock_);
  if (last_state_.error != gpu::error::kNoError)
    return;

  Send(new GpuCommandBufferMsg_SetGetBuffer(route_id_, shm_id));
  last_put_offset_ = -1;
  has_buffer_ = (shm_id > 0);
}
