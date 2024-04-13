RenderThreadImpl::~RenderThreadImpl() {
  for (std::map<int, mojo::MessagePipeHandle>::iterator it =
           pending_render_frame_connects_.begin();
       it != pending_render_frame_connects_.end();
       ++it) {
    mojo::CloseRaw(it->second);
  }
}
