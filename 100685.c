void WebContentsImpl::ResizeDueToAutoResize(const gfx::Size& new_size) {
  if (delegate_)
    delegate_->ResizeDueToAutoResize(this, new_size);
}
