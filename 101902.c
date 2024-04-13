void DelegatedFrameHost::ResetCompositor() {
  if (!compositor_)
    return;
  resize_lock_.reset();
  if (compositor_->HasObserver(this))
    compositor_->RemoveObserver(this);
  if (vsync_manager_) {
    vsync_manager_->RemoveObserver(this);
    vsync_manager_ = nullptr;
  }

  compositor_->RemoveFrameSink(frame_sink_id_);
  compositor_ = nullptr;
}
