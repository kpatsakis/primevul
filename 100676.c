void WebContentsImpl::OnIgnoredUIEvent() {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_, DidGetIgnoredUIEvent());
}
