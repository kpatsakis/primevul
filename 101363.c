bool RenderWidgetHostImpl::CanCopyFromBackingStore() {
  if (view_)
    return view_->IsSurfaceAvailableForCopy();
  return false;
}
