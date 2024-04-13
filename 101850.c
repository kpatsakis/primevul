void CommandBufferProxyImpl::OnConsoleMessage(
    const GPUCommandBufferConsoleMessage& message) {
  if (gpu_control_client_)
    gpu_control_client_->OnGpuControlErrorMessage(message.message.c_str(),
                                                  message.id);
}
