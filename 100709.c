scoped_ptr<gfx::GpuMemoryBuffer> RenderThreadImpl::AllocateGpuMemoryBuffer(
    size_t width,
    size_t height,
    unsigned internalformat,
    unsigned usage) {
  DCHECK(allocate_gpu_memory_buffer_thread_checker_.CalledOnValidThread());

  if (!GpuMemoryBufferImpl::IsFormatValid(internalformat))
    return scoped_ptr<gfx::GpuMemoryBuffer>();

  gfx::GpuMemoryBufferHandle handle;
  bool success;
  IPC::Message* message = new ChildProcessHostMsg_SyncAllocateGpuMemoryBuffer(
      width, height, internalformat, usage, &handle);

  if (base::MessageLoop::current() == message_loop())
    success = ChildThread::Send(message);
  else
    success = sync_message_filter()->Send(message);

  if (!success)
    return scoped_ptr<gfx::GpuMemoryBuffer>();

  return GpuMemoryBufferImpl::CreateFromHandle(
             handle, gfx::Size(width, height), internalformat)
      .PassAs<gfx::GpuMemoryBuffer>();
}
