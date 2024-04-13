void SendCompositorFrameAck(
    int32 route_id,
    uint32 output_surface_id,
    int renderer_host_id,
    const gpu::Mailbox& received_mailbox,
    const gfx::Size& received_size,
    bool skip_frame,
    const scoped_refptr<ui::Texture>& texture_to_produce) {
  cc::CompositorFrameAck ack;
  ack.gl_frame_data.reset(new cc::GLFrameData());
  DCHECK(!texture_to_produce.get() || !skip_frame);
  if (texture_to_produce.get()) {
    GLHelper* gl_helper = ImageTransportFactory::GetInstance()->GetGLHelper();
    std::string mailbox_name = texture_to_produce->Produce();
    std::copy(mailbox_name.data(),
              mailbox_name.data() + mailbox_name.length(),
              reinterpret_cast<char*>(ack.gl_frame_data->mailbox.name));
    ack.gl_frame_data->size = texture_to_produce->size();
    ack.gl_frame_data->sync_point =
        gl_helper ? gl_helper->InsertSyncPoint() : 0;
  } else if (skip_frame) {
    ack.gl_frame_data->size = received_size;
    ack.gl_frame_data->mailbox = received_mailbox;
  }

  RenderWidgetHostImpl::SendSwapCompositorFrameAck(
      route_id, output_surface_id, renderer_host_id, ack);
}
