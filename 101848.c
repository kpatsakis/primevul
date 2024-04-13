void CommandBufferProxyImpl::DisconnectChannelInFreshCallStack() {
  CheckLock();
  last_state_lock_.AssertAcquired();
  if (gpu_control_client_)
    gpu_control_client_->OnGpuControlLostContextMaybeReentrant();
  callback_thread_->PostTask(
      FROM_HERE, base::Bind(&CommandBufferProxyImpl::LockAndDisconnectChannel,
                            weak_ptr_factory_.GetWeakPtr()));
}
