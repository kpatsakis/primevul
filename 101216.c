RenderThreadImpl::GetGpuFactories() {
  DCHECK(IsMainThread());

  scoped_refptr<GpuChannelHost> gpu_channel_host = GetGpuChannel();
  const CommandLine* cmd_line = CommandLine::ForCurrentProcess();
  scoped_refptr<media::GpuVideoAcceleratorFactories> gpu_factories;
  scoped_refptr<base::SingleThreadTaskRunner> media_task_runner =
      GetMediaThreadTaskRunner();
  if (!cmd_line->HasSwitch(switches::kDisableAcceleratedVideoDecode)) {
    if (!gpu_va_context_provider_.get() ||
        gpu_va_context_provider_->DestroyedOnMainThread()) {
      if (!gpu_channel_host.get()) {
        gpu_channel_host = EstablishGpuChannelSync(
            CAUSE_FOR_GPU_LAUNCH_WEBGRAPHICSCONTEXT3DCOMMANDBUFFERIMPL_INITIALIZE);
      }
      blink::WebGraphicsContext3D::Attributes attributes;
      bool lose_context_when_out_of_memory = false;
      gpu_va_context_provider_ = ContextProviderCommandBuffer::Create(
          make_scoped_ptr(
              WebGraphicsContext3DCommandBufferImpl::CreateOffscreenContext(
                  gpu_channel_host.get(),
                  attributes,
                  lose_context_when_out_of_memory,
                  GURL("chrome://gpu/RenderThreadImpl::GetGpuVDAContext3D"),
                  WebGraphicsContext3DCommandBufferImpl::SharedMemoryLimits(),
                  NULL)),
          "GPU-VideoAccelerator-Offscreen");
    }
  }
  if (gpu_va_context_provider_.get()) {
    gpu_factories = RendererGpuVideoAcceleratorFactories::Create(
        gpu_channel_host.get(), media_task_runner, gpu_va_context_provider_);
  }
  return gpu_factories;
}
