scoped_ptr<base::SharedMemory> AllocateSharedMemoryFunction(size_t size) {
  return RenderThreadImpl::Get()->HostAllocateSharedMemoryBuffer(size);
}
