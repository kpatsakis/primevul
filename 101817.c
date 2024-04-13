void DiscardableSharedMemoryManager::OnMemoryStateChange(
    base::MemoryState state) {
  base::AutoLock lock(lock_);
  switch (state) {
    case base::MemoryState::NORMAL:
      memory_limit_ = default_memory_limit_;
      break;
    case base::MemoryState::THROTTLED:
      memory_limit_ = 0;
      break;
    case base::MemoryState::SUSPENDED:
    case base::MemoryState::UNKNOWN:
      NOTREACHED();
      break;
  }
}
