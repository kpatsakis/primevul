void RenderWidgetHostImpl::SetNeedsBeginFrameForFlingProgress() {
  browser_fling_needs_begin_frame_ = true;
  SetNeedsBeginFrame(true);
}
