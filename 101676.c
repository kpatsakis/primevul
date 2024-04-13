void RenderWidgetHostImpl::SubmitCompositorFrame(
    const viz::LocalSurfaceId& local_surface_id,
    viz::CompositorFrame frame,
    base::Optional<viz::HitTestRegionList> hit_test_region_list,
    uint64_t submit_time) {
  if (frame.HasCopyOutputRequests()) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RWH_COPY_REQUEST_ATTEMPT);
    return;
  }

  auto new_surface_properties =
      RenderWidgetSurfaceProperties::FromCompositorFrame(frame);

  if (local_surface_id == last_local_surface_id_ &&
      new_surface_properties != last_surface_properties_) {
    std::string error = base::StringPrintf(
        "[OOPIF? %d] %s\n", view_ && view_->IsRenderWidgetHostViewChildFrame(),
        new_surface_properties.ToDiffString(last_surface_properties_).c_str());
    LOG(ERROR) << "Surface invariants violation: " << error;

    static int invariants_violation_count = 0;
    ++invariants_violation_count;
    UMA_HISTOGRAM_COUNTS_1000("Compositing.SurfaceInvariantsViolations",
                              invariants_violation_count);

    if (features::IsSurfaceInvariantsViolationLoggingEnabled()) {
      static auto* crash_key = base::debug::AllocateCrashKeyString(
          "surface-invariants-violation", base::debug::CrashKeySize::Size256);
      base::debug::ScopedCrashKeyString key_value(crash_key, error);
      base::debug::DumpWithoutCrashing();
    }

    if (view_) {
      frame.metadata.begin_frame_ack.has_damage = false;
      view_->OnDidNotProduceFrame(frame.metadata.begin_frame_ack);
    }
    std::vector<viz::ReturnedResource> resources =
        viz::TransferableResource::ReturnResources(frame.resource_list);
    renderer_compositor_frame_sink_->DidReceiveCompositorFrameAck(resources);

    return;
  }

  last_local_surface_id_ = local_surface_id;
  last_surface_properties_ = new_surface_properties;

  last_received_content_source_id_ = frame.metadata.content_source_id;

  frame.metadata.begin_frame_ack.has_damage = true;

  last_frame_metadata_ = frame.metadata.Clone();

  if (enable_surface_synchronization_) {
    if (view_) {
      view_->SubmitCompositorFrame(local_surface_id, std::move(frame),
                                   std::move(hit_test_region_list));
      view_->DidReceiveRendererFrame();
    } else {
      std::vector<viz::ReturnedResource> resources =
          viz::TransferableResource::ReturnResources(frame.resource_list);
      renderer_compositor_frame_sink_->DidReceiveCompositorFrameAck(resources);
    }
  } else {
    if (view_ &&
        frame.metadata.content_source_id >= current_content_source_id_) {
      view_->SubmitCompositorFrame(local_surface_id, std::move(frame),
                                   std::move(hit_test_region_list));
      view_->DidReceiveRendererFrame();
    } else {
      if (view_) {
        frame.metadata.begin_frame_ack.has_damage = false;
        view_->OnDidNotProduceFrame(frame.metadata.begin_frame_ack);
      }
      std::vector<viz::ReturnedResource> resources =
          viz::TransferableResource::ReturnResources(frame.resource_list);
      renderer_compositor_frame_sink_->DidReceiveCompositorFrameAck(resources);
    }

    if (last_received_content_source_id_ >= current_content_source_id_) {
      did_receive_first_frame_after_navigation_ = true;
      if (new_content_rendering_timeout_ &&
          new_content_rendering_timeout_->IsRunning()) {
        new_content_rendering_timeout_->Stop();
      }
    }
  }
}
