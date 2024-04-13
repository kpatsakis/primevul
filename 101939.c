void NavigationRequest::OnRequestFailedInternal(
    const network::URLLoaderCompletionStatus& status,
    bool skip_throttles,
    const base::Optional<std::string>& error_page_content,
    bool collapse_frame) {
  DCHECK(state_ == STARTED || state_ == RESPONSE_STARTED);
  DCHECK(!(status.error_code == net::ERR_ABORTED &&
           error_page_content.has_value()));
  common_params_.previews_state = content::PREVIEWS_OFF;
  if (status.ssl_info.has_value())
    ssl_info_ = status.ssl_info;

  devtools_instrumentation::OnNavigationRequestFailed(*this, status);

  TRACE_EVENT_ASYNC_STEP_INTO1("navigation", "NavigationRequest", this,
                               "OnRequestFailed", "error", status.error_code);
  state_ = FAILED;
  if (navigation_handle_.get()) {
    navigation_handle_->set_net_error_code(
        static_cast<net::Error>(status.error_code));
  }

  int expected_pending_entry_id =
      navigation_handle_.get() ? navigation_handle_->pending_nav_entry_id()
                               : nav_entry_id_;
  frame_tree_node_->navigator()->DiscardPendingEntryIfNeeded(
      expected_pending_entry_id);

  if (status.error_code == net::ERR_ABORTED) {
    frame_tree_node_->ResetNavigationRequest(false, true);
    return;
  }

  if (collapse_frame) {
    DCHECK(!frame_tree_node_->IsMainFrame());
    DCHECK_EQ(net::ERR_BLOCKED_BY_CLIENT, status.error_code);
    frame_tree_node_->SetCollapsed(true);
  }

  RenderFrameHostImpl* render_frame_host = nullptr;
  if (SiteIsolationPolicy::IsErrorPageIsolationEnabled(
          frame_tree_node_->IsMainFrame())) {
    navigation_handle_->SetExpectedProcess(nullptr);
    render_frame_host =
        frame_tree_node_->render_manager()->GetFrameHostForNavigation(*this);
  } else {
    if (ShouldKeepErrorPageInCurrentProcess(status.error_code)) {
      render_frame_host = frame_tree_node_->current_frame_host();
    } else {
      render_frame_host =
          frame_tree_node_->render_manager()->GetFrameHostForNavigation(*this);
    }
  }

  CHECK(!render_frame_host_ || render_frame_host_ == render_frame_host);
  render_frame_host_ = render_frame_host;

  DCHECK(render_frame_host_);

  NavigatorImpl::CheckWebUIRendererDoesNotDisplayNormalURL(render_frame_host_,
                                                           common_params_.url);

  has_stale_copy_in_cache_ = status.exists_in_cache;
  net_error_ = status.error_code;

  if (skip_throttles) {
    CommitErrorPage(error_page_content);
  } else {
    navigation_handle_->WillFailRequest(base::BindOnce(
        &NavigationRequest::OnFailureChecksComplete, base::Unretained(this)));
  }
}
