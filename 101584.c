bool HistoryController::GoToEntry(
    blink::WebLocalFrame* main_frame,
    scoped_ptr<HistoryEntry> target_entry,
    scoped_ptr<NavigationParams> navigation_params,
    WebURLRequest::CachePolicy cache_policy) {
  DCHECK(!main_frame->parent());
  HistoryFrameLoadVector same_document_loads;
  HistoryFrameLoadVector different_document_loads;

  set_provisional_entry(std::move(target_entry));
  navigation_params_ = std::move(navigation_params);

  if (current_entry_) {
    RecursiveGoToEntry(
        main_frame, same_document_loads, different_document_loads);
  }

  if (same_document_loads.empty() && different_document_loads.empty()) {
    different_document_loads.push_back(
        std::make_pair(main_frame, provisional_entry_->root()));
  }

  bool has_main_frame_request = false;
  for (const auto& item : same_document_loads) {
    WebFrame* frame = item.first;
    RenderFrameImpl* render_frame = RenderFrameImpl::FromWebFrame(frame);
    if (!render_frame)
      continue;
    render_frame->SetPendingNavigationParams(make_scoped_ptr(
        new NavigationParams(*navigation_params_.get())));
    WebURLRequest request = frame->toWebLocalFrame()->requestFromHistoryItem(
        item.second, cache_policy);
    frame->toWebLocalFrame()->load(
        request, blink::WebFrameLoadType::BackForward, item.second,
        blink::WebHistorySameDocumentLoad);
    if (frame == main_frame)
      has_main_frame_request = true;
  }
  for (const auto& item : different_document_loads) {
    WebFrame* frame = item.first;
    RenderFrameImpl* render_frame = RenderFrameImpl::FromWebFrame(frame);
    if (!render_frame)
      continue;
    render_frame->SetPendingNavigationParams(make_scoped_ptr(
        new NavigationParams(*navigation_params_.get())));
    WebURLRequest request = frame->toWebLocalFrame()->requestFromHistoryItem(
        item.second, cache_policy);
    frame->toWebLocalFrame()->load(
        request, blink::WebFrameLoadType::BackForward, item.second,
        blink::WebHistoryDifferentDocumentLoad);
    if (frame == main_frame)
      has_main_frame_request = true;
  }

  return has_main_frame_request;
}
