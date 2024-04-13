void ClientDiscardableSharedMemoryManager::MemoryUsageChanged(
    size_t new_bytes_total,
    size_t new_bytes_free) const {
  static crash_reporter::CrashKeyString<24> discardable_memory_allocated(
      "discardable-memory-allocated");
  discardable_memory_allocated.Set(base::NumberToString(new_bytes_total));

  static crash_reporter::CrashKeyString<24> discardable_memory_free(
      "discardable-memory-free");
  discardable_memory_free.Set(base::NumberToString(new_bytes_free));
}
