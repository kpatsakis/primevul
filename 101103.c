void AcknowledgeBufferForGpu(
    int32 route_id,
    int gpu_host_id,
    const std::string& received_mailbox,
    bool skip_frame,
    const scoped_refptr<ui::Texture>& texture_to_produce) {
  AcceleratedSurfaceMsg_BufferPresented_Params ack;
  uint32 sync_point = 0;
  DCHECK(!texture_to_produce.get() || !skip_frame);
  if (texture_to_produce.get()) {
    GLHelper* gl_helper = ImageTransportFactory::GetInstance()->GetGLHelper();
    ack.mailbox_name = texture_to_produce->Produce();
    sync_point = gl_helper ? gl_helper->InsertSyncPoint() : 0;
  } else if (skip_frame) {
    ack.mailbox_name = received_mailbox;
    ack.sync_point = 0;
  }

  ack.sync_point = sync_point;
  RenderWidgetHostImpl::AcknowledgeBufferPresent(
      route_id, gpu_host_id, ack);
}
