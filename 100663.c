void WebContentsImpl::DidAccessInitialDocument() {
  NotifyNavigationStateChanged(content::INVALIDATE_TYPE_URL);
}
