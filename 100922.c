bool RenderViewImpl::GetPpapiPluginCaretBounds(gfx::Rect* rect) {
  if (!pepper_helper_->IsPluginFocused())
    return false;
  *rect = pepper_helper_->GetCaretBounds();
  return true;
}
