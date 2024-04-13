bool MemBackendImpl::Init() {
  if (max_size_)
    return true;

  int64_t total_memory = base::SysInfo::AmountOfPhysicalMemory();

  if (total_memory <= 0) {
    max_size_ = kDefaultInMemoryCacheSize;
    return true;
  }

  total_memory = total_memory * 2 / 100;
  if (total_memory > kDefaultInMemoryCacheSize * 5)
    max_size_ = kDefaultInMemoryCacheSize * 5;
  else
    max_size_ = static_cast<int32_t>(total_memory);

  return true;
}
