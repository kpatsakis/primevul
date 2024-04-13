void NavigationRequest::OnRequestStarted(base::TimeTicks timestamp) {
  if (frame_tree_node_->IsMainFrame()) {
    TRACE_EVENT_ASYNC_END_WITH_TIMESTAMP0(
        "navigation", "Navigation timeToNetworkStack", navigation_handle_.get(),
        timestamp);
  }

  frame_tree_node_->navigator()->LogResourceRequestTime(timestamp,
                                                        common_params_.url);
}
