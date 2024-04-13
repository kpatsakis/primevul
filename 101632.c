void NavigatorImpl::Navigate(std::unique_ptr<NavigationRequest> request,
                             ReloadType reload_type,
                             RestoreType restore_type) {
  TRACE_EVENT0("browser,navigation", "NavigatorImpl::Navigate");

  const GURL& dest_url = request->common_params().url;
  FrameTreeNode* frame_tree_node = request->frame_tree_node();

  navigation_data_.reset(new NavigationMetricsData(
      request->common_params().navigation_start, dest_url, restore_type));

  bool should_dispatch_beforeunload =
      !FrameMsg_Navigate_Type::IsSameDocument(
          request->common_params().navigation_type) &&
      !request->request_params().is_history_navigation_in_new_child &&
      frame_tree_node->current_frame_host()->ShouldDispatchBeforeUnload(
          false /* check_subframes_only */);

  int nav_entry_id = request->nav_entry_id();
  bool is_pending_entry =
      controller_->GetPendingEntry() &&
      (nav_entry_id == controller_->GetPendingEntry()->GetUniqueID());
  frame_tree_node->CreatedNavigationRequest(std::move(request));
  DCHECK(frame_tree_node->navigation_request());

  if (should_dispatch_beforeunload) {
    frame_tree_node->navigation_request()->SetWaitingForRendererResponse();
    frame_tree_node->current_frame_host()->DispatchBeforeUnload(
        RenderFrameHostImpl::BeforeUnloadType::BROWSER_INITIATED_NAVIGATION,
        reload_type != ReloadType::NONE);
  } else {
    frame_tree_node->navigation_request()->BeginNavigation();
  }

  if (is_pending_entry)
    CHECK_EQ(nav_entry_id, controller_->GetPendingEntry()->GetUniqueID());

  if (delegate_ && is_pending_entry)
    delegate_->DidStartNavigationToPendingEntry(dest_url, reload_type);
}
