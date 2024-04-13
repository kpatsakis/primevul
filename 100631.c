void GLES2DecoderImpl::UpdateBackbufferMemoryAccounting() {
  size_t total = 0;
  if (offscreen_target_frame_buffer_.get()) {
    if (offscreen_target_color_texture_.get()) {
        total += offscreen_target_color_texture_->estimated_size();
    }
    if (offscreen_target_color_render_buffer_.get()) {
        total += offscreen_target_color_render_buffer_->estimated_size();
    }
    if (offscreen_target_depth_render_buffer_.get()) {
        total += offscreen_target_depth_render_buffer_->estimated_size();
    }
    if (offscreen_target_stencil_render_buffer_.get()) {
        total += offscreen_target_stencil_render_buffer_->estimated_size();
    }
    if (offscreen_saved_color_texture_.get()) {
        total += offscreen_saved_color_texture_->estimated_size();
    }
    if (offscreen_resolved_color_texture_.get()) {
        total += offscreen_resolved_color_texture_->estimated_size();
    }
  } else {
    gfx::Size size = surface_->GetSize();
    total += size.width() * size.height() *
        GLES2Util::RenderbufferBytesPerPixel(back_buffer_color_format_);
  }
  TRACE_COUNTER_ID1(
      "GLES2DecoderImpl", "BackbufferMemory", this, total);
}
