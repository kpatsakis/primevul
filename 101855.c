void CommandBufferProxyImpl::SetNeedsVSync(bool needs_vsync) {
  CheckLock();
  base::AutoLock lock(last_state_lock_);
  if (last_state_.error != gpu::error::kNoError)
    return;

  Send(new GpuCommandBufferMsg_SetNeedsVSync(route_id_, needs_vsync));
}
