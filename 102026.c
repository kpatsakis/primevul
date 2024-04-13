void RenderWidgetHostImpl::SetNeedsBeginFrame(bool needs_begin_frames) {
  if (needs_begin_frames_ == needs_begin_frames)
    return;

  needs_begin_frames_ = needs_begin_frames;
  if (view_)
    view_->SetNeedsBeginFrames(needs_begin_frames);
}
