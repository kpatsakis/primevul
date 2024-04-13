void WebContentsImpl::OnJSOutOfMemory() {
  if (delegate_)
    delegate_->JSOutOfMemory(this);
}
