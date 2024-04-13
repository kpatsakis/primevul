void WebContentsImpl::RenderWidgetDeleted(
    RenderWidgetHostImpl* render_widget_host) {
  if (is_being_destroyed_) {
    return;
  }

  std::set<RenderWidgetHostImpl*>::iterator iter =
      created_widgets_.find(render_widget_host);
  if (iter != created_widgets_.end())
    created_widgets_.erase(iter);

  if (render_widget_host &&
      render_widget_host->GetRoutingID() == fullscreen_widget_routing_id_) {
    if (delegate_ && delegate_->EmbedsFullscreenWidget())
      delegate_->ToggleFullscreenModeForTab(this, false);
    FOR_EACH_OBSERVER(WebContentsObserver,
                      observers_,
                      DidDestroyFullscreenWidget(
                          fullscreen_widget_routing_id_));
    fullscreen_widget_routing_id_ = MSG_ROUTING_NONE;
  }
}
