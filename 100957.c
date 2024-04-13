RenderViewImpl::~RenderViewImpl() {
  history_page_ids_.clear();

  if (decrement_shared_popup_at_destruction_)
    shared_popup_counter_->data--;

  while (!file_chooser_completions_.empty()) {
    if (file_chooser_completions_.front()->completion) {
      file_chooser_completions_.front()->completion->didChooseFile(
          WebVector<WebString>());
    }
    file_chooser_completions_.pop_front();
  }

#if defined(OS_MACOSX)
  while (!fake_plugin_window_handles_.empty()) {
    DCHECK(*fake_plugin_window_handles_.begin());
    DestroyFakePluginWindowHandle(*fake_plugin_window_handles_.begin());
  }
#endif

#ifndef NDEBUG
  ViewMap* views = g_view_map.Pointer();
  for (ViewMap::iterator it = views->begin(); it != views->end(); ++it)
    DCHECK_NE(this, it->second) << "Failed to call Close?";
  RoutingIDViewMap* routing_id_views = g_routing_id_view_map.Pointer();
  for (RoutingIDViewMap::iterator it = routing_id_views->begin();
       it != routing_id_views->end(); ++it)
    DCHECK_NE(this, it->second) << "Failed to call Close?";
#endif

  FOR_EACH_OBSERVER(RenderViewObserver, observers_, RenderViewGone());
  FOR_EACH_OBSERVER(RenderViewObserver, observers_, OnDestruct());
}
