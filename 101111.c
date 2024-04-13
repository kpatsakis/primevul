void RenderWidgetHostViewAura::OnSwapCompositorFrame(
    uint32 output_surface_id,
    scoped_ptr<cc::CompositorFrame> frame) {
  TRACE_EVENT0("content", "RenderWidgetHostViewAura::OnSwapCompositorFrame");
  if (frame->delegated_frame_data) {
    SwapDelegatedFrame(output_surface_id,
                       frame->delegated_frame_data.Pass(),
                       frame->metadata.device_scale_factor,
                       frame->metadata.latency_info);
    return;
  }

  if (frame->software_frame_data) {
    SwapSoftwareFrame(output_surface_id,
                      frame->software_frame_data.Pass(),
                      frame->metadata.device_scale_factor,
                      frame->metadata.latency_info);
    return;
  }

  if (!frame->gl_frame_data || frame->gl_frame_data->mailbox.IsZero())
    return;

  BufferPresentedCallback ack_callback = base::Bind(
      &SendCompositorFrameAck,
      host_->GetRoutingID(), output_surface_id, host_->GetProcess()->GetID(),
      frame->gl_frame_data->mailbox, frame->gl_frame_data->size);

  if (!frame->gl_frame_data->sync_point) {
    LOG(ERROR) << "CompositorFrame without sync point. Skipping frame...";
    ack_callback.Run(true, scoped_refptr<ui::Texture>());
    return;
  }

  GLHelper* gl_helper = ImageTransportFactory::GetInstance()->GetGLHelper();
  gl_helper->WaitSyncPoint(frame->gl_frame_data->sync_point);

  std::string mailbox_name(
      reinterpret_cast<const char*>(frame->gl_frame_data->mailbox.name),
      sizeof(frame->gl_frame_data->mailbox.name));
  BuffersSwapped(frame->gl_frame_data->size,
                 frame->gl_frame_data->sub_buffer_rect,
                 frame->metadata.device_scale_factor,
                 mailbox_name,
                 frame->metadata.latency_info,
                 ack_callback);
}
