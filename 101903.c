bool DelegatedFrameHost::ShouldSkipFrame(const gfx::Size& size_in_dip) {
  if (!resize_lock_)
    return false;
  if (allow_one_renderer_frame_during_resize_lock_) {
    allow_one_renderer_frame_during_resize_lock_ = false;
    return false;
  }
  return size_in_dip != resize_lock_->expected_size();
}
