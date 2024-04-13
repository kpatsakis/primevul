void RenderViewImpl::willSendRequest(WebFrame* frame,
                                     unsigned identifier,
                                     WebURLRequest& request,
                                     const WebURLResponse& redirect_response) {
  WebFrame* top_frame = frame->top();
  if (!top_frame)
    top_frame = frame;
  WebDataSource* provisional_data_source = top_frame->provisionalDataSource();
  WebDataSource* top_data_source = top_frame->dataSource();
  WebDataSource* data_source =
      provisional_data_source ? provisional_data_source : top_data_source;

  PageTransition transition_type = PAGE_TRANSITION_LINK;
  DocumentState* document_state = DocumentState::FromDataSource(data_source);
  DCHECK(document_state);
  NavigationState* navigation_state = document_state->navigation_state();
  transition_type = navigation_state->transition_type();

  GURL request_url(request.url());
  GURL new_url;
  if (GetContentClient()->renderer()->WillSendRequest(
          frame,
          transition_type,
          request_url,
          request.firstPartyForCookies(),
          &new_url)) {
    request.setURL(WebURL(new_url));
  }

  if (document_state->is_cache_policy_override_set())
    request.setCachePolicy(document_state->cache_policy_override());

  WebKit::WebReferrerPolicy referrer_policy;
  if (document_state && document_state->is_referrer_policy_set()) {
    referrer_policy = document_state->referrer_policy();
    document_state->clear_referrer_policy();
  } else {
    referrer_policy = frame->document().referrerPolicy();
  }

  WebString custom_user_agent;
  if (request.extraData()) {
    webkit_glue::WebURLRequestExtraDataImpl* old_extra_data =
        static_cast<webkit_glue::WebURLRequestExtraDataImpl*>(
            request.extraData());
    custom_user_agent = old_extra_data->custom_user_agent();

    if (!custom_user_agent.isNull()) {
      if (custom_user_agent.isEmpty())
        request.clearHTTPHeaderField("User-Agent");
      else
        request.setHTTPHeaderField("User-Agent", custom_user_agent);
    }
  }

  request.setExtraData(
      new RequestExtraData(referrer_policy,
                           custom_user_agent,
                           (frame == top_frame),
                           frame->identifier(),
                           frame->parent() == top_frame,
                           frame->parent() ? frame->parent()->identifier() : -1,
                           navigation_state->allow_download(),
                           transition_type,
                           navigation_state->transferred_request_child_id(),
                           navigation_state->transferred_request_request_id()));

  DocumentState* top_document_state =
      DocumentState::FromDataSource(top_data_source);
  if (top_document_state &&
      request.targetType() == WebURLRequest::TargetIsPrefetch)
    top_document_state->set_was_prefetcher(true);

  request.setRequestorID(routing_id_);
  request.setHasUserGesture(frame->isProcessingUserGesture());

  if (!renderer_preferences_.enable_referrers)
    request.clearHTTPHeaderField("Referer");
}
