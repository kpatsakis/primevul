ContextResult CommandBufferProxyImpl::Initialize(
    gpu::SurfaceHandle surface_handle,
    CommandBufferProxyImpl* share_group,
    gpu::SchedulingPriority stream_priority,
    const gpu::ContextCreationAttribs& attribs,
    const GURL& active_url) {
  DCHECK(!share_group || (stream_id_ == share_group->stream_id_));
  TRACE_EVENT1("gpu", "GpuChannelHost::CreateViewCommandBuffer",
               "surface_handle", surface_handle);

  auto channel = std::move(channel_);

  GPUCreateCommandBufferConfig init_params;
  init_params.surface_handle = surface_handle;
  init_params.share_group_id =
      share_group ? share_group->route_id_ : MSG_ROUTING_NONE;
  init_params.stream_id = stream_id_;
  init_params.stream_priority = stream_priority;
  init_params.attribs = attribs;
  init_params.active_url = active_url;

  TRACE_EVENT0("gpu", "CommandBufferProxyImpl::Initialize");
  shared_state_shm_ = AllocateAndMapSharedMemory(sizeof(*shared_state()));
  if (!shared_state_shm_) {
    LOG(ERROR) << "ContextResult::kFatalFailure: "
                  "AllocateAndMapSharedMemory failed";
    return ContextResult::kFatalFailure;
  }

  shared_state()->Initialize();

  base::SharedMemoryHandle handle =
      channel->ShareToGpuProcess(shared_state_shm_->handle());
  if (!base::SharedMemory::IsHandleValid(handle)) {
    LOG(ERROR) << "ContextResult::kFatalFailure: "
                  "Shared memory handle is not valid";
    return ContextResult::kFatalFailure;
  }

  channel->AddRouteWithTaskRunner(route_id_, weak_ptr_factory_.GetWeakPtr(),
                                  callback_thread_);

  ContextResult result = ContextResult::kSuccess;
  bool sent = channel->Send(new GpuChannelMsg_CreateCommandBuffer(
      init_params, route_id_, handle, &result, &capabilities_));
  if (!sent) {
    channel->RemoveRoute(route_id_);
    LOG(ERROR) << "ContextResult::kTransientFailure: "
                  "Failed to send GpuChannelMsg_CreateCommandBuffer.";
    return ContextResult::kTransientFailure;
  }
  if (result != ContextResult::kSuccess) {
    DLOG(ERROR) << "Failure processing GpuChannelMsg_CreateCommandBuffer.";
    channel->RemoveRoute(route_id_);
    return result;
  }

  channel_ = std::move(channel);
  return result;
}
