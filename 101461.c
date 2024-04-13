void SkiaOutputSurfaceImpl::InitializeOnGpuThread(base::WaitableEvent* event,
                                                  bool* result) {
  base::Optional<base::ScopedClosureRunner> scoped_runner;
  if (event) {
    scoped_runner.emplace(
        base::BindOnce(&base::WaitableEvent::Signal, base::Unretained(event)));
  }

  auto did_swap_buffer_complete_callback = CreateSafeCallback(
      dependency_.get(),
      base::BindRepeating(&SkiaOutputSurfaceImpl::DidSwapBuffersComplete,
                          weak_ptr_));
  auto buffer_presented_callback = CreateSafeCallback(
      dependency_.get(),
      base::BindRepeating(&SkiaOutputSurfaceImpl::BufferPresented, weak_ptr_));
  auto context_lost_callback = CreateSafeCallback(
      dependency_.get(),
      base::BindRepeating(&SkiaOutputSurfaceImpl::ContextLost, weak_ptr_));
  auto gpu_vsync_callback = CreateSafeCallback(
      dependency_.get(),
      base::BindRepeating(&SkiaOutputSurfaceImpl::OnGpuVSync, weak_ptr_));

  impl_on_gpu_ = SkiaOutputSurfaceImplOnGpu::Create(
      dependency_.get(), renderer_settings_, task_sequence_->GetSequenceId(),
      std::move(did_swap_buffer_complete_callback),
      std::move(buffer_presented_callback), std::move(context_lost_callback),
      std::move(gpu_vsync_callback));
  if (!impl_on_gpu_) {
    *result = false;
  } else {
    capabilities_ = impl_on_gpu_->capabilities();
    capabilities_.android_surface_control_feature_enabled =
        dependency_->GetGpuFeatureInfo()
            .status_values[gpu::GPU_FEATURE_TYPE_ANDROID_SURFACE_CONTROL] ==
        gpu::kGpuFeatureStatusEnabled;

    is_displayed_as_overlay_ = impl_on_gpu_->IsDisplayedAsOverlay();
    *result = true;
  }
}
