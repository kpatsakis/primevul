void RenderView::didFailProvisionalLoad(WebFrame* frame,
                                        const WebURLError& error) {
  WebDataSource* ds = frame->provisionalDataSource();
  DCHECK(ds);

  const WebURLRequest& failed_request = ds->request();

  bool show_repost_interstitial =
      (error.reason == net::ERR_CACHE_MISS &&
       EqualsASCII(failed_request.httpMethod(), "POST"));
  Send(new ViewHostMsg_DidFailProvisionalLoadWithError(
      routing_id_, !frame->parent(), error.reason, error.unreachableURL,
      show_repost_interstitial));

  if (error.reason == net::ERR_ABORTED)
    return;

  frame->enableViewSourceMode(false);

  NavigationState* navigation_state = NavigationState::FromDataSource(ds);

  bool replace =
      navigation_state->pending_page_id() != -1 ||
      navigation_state->transition_type() == PageTransition::AUTO_SUBFRAME;

  if (!navigation_state->is_content_initiated()) {
    pending_navigation_state_.reset(NavigationState::CreateBrowserInitiated(
        navigation_state->pending_page_id(),
        navigation_state->pending_history_list_offset(),
        navigation_state->transition_type(),
        navigation_state->request_time()));
  }

  if (MaybeLoadAlternateErrorPage(frame, error, replace))
    return;

  LoadNavigationErrorPage(frame, failed_request, error, std::string(),
                          replace);
}
