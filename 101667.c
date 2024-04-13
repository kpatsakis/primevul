void RenderWidgetHostImpl::OnFocusedNodeTouched(bool editable) {
  if (delegate_)
    delegate_->FocusedNodeTouched(editable);
}
