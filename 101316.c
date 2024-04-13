void EventBindings::DetachFilteredEvent(int matcher_id, bool is_manual) {
  EventFilter& event_filter = g_event_filter.Get();
  EventMatcher* event_matcher = event_filter.GetEventMatcher(matcher_id);

  const std::string& event_name = event_filter.GetEventName(matcher_id);

  std::string extension_id = context()->GetExtensionID();
  if (RemoveFilter(event_name, extension_id, event_matcher->value())) {
    bool remove_lazy =
        is_manual && ExtensionFrameHelper::IsContextForEventPage(context());
    content::RenderThread::Get()->Send(
        new ExtensionHostMsg_RemoveFilteredListener(
            extension_id, event_name, *event_matcher->value(), remove_lazy));
  }

  event_filter.RemoveEventMatcher(matcher_id);
  attached_matcher_ids_.erase(matcher_id);
}
