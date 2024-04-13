void WebContentsImpl::WasOccluded() {
  const Visibility previous_visibility = GetVisibility();

  if (!IsBeingCaptured()) {
    for (RenderWidgetHostView* view : GetRenderWidgetHostViewsInTree())
      view->WasOccluded();
  }

  should_normally_be_occluded_ = true;
  NotifyVisibilityChanged(previous_visibility);
}
