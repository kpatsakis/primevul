ClientDiscardableSharedMemoryManager::GetStatistics() const {
  base::AutoLock lock(lock_);
  Statistics stats;
  stats.total_size = heap_->GetSize();
  stats.freelist_size = heap_->GetSizeOfFreeLists();
  return stats;
}
