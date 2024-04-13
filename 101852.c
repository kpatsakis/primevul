void CommandBufferProxyImpl::OnSignalAck(uint32_t id,
                                         const CommandBuffer::State& state) {
  {
    base::AutoLock lock(last_state_lock_);
    SetStateFromMessageReply(state);
    if (last_state_.error != gpu::error::kNoError)
      return;
  }
  SignalTaskMap::iterator it = signal_tasks_.find(id);
  if (it == signal_tasks_.end()) {
    LOG(ERROR) << "Gpu process sent invalid SignalAck.";
    base::AutoLock lock(last_state_lock_);
    OnGpuAsyncMessageError(gpu::error::kInvalidGpuMessage,
                           gpu::error::kLostContext);
    return;
  }
  base::Closure callback = it->second;
  signal_tasks_.erase(it);
  callback.Run();
}
