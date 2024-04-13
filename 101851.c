void CommandBufferProxyImpl::OnGpuSyncReplyError() {
  CheckLock();
  last_state_lock_.AssertAcquired();
  last_state_.error = gpu::error::kLostContext;
  last_state_.context_lost_reason = gpu::error::kInvalidGpuMessage;
  DisconnectChannelInFreshCallStack();
}
