void RenderViewImpl::didStartProvisionalLoad(WebFrame* frame) {
  WebDataSource* ds = frame->provisionalDataSource();

  if (!ds)
    return;

  DocumentState* document_state = DocumentState::FromDataSource(ds);

  CHECK((ds->request().url() != GURL(kSwappedOutURL)) ||
        is_swapped_out_) << "Heard swappedout:// when not swapped out.";

  if (document_state->request_time().is_null()) {
    double event_time = ds->triggeringEventTime();
    if (event_time != 0.0)
      document_state->set_request_time(Time::FromDoubleT(event_time));
  }

  document_state->set_start_load_time(Time::Now());

  bool is_top_most = !frame->parent();
  if (is_top_most) {
    navigation_gesture_ = frame->isProcessingUserGesture() ?
        NavigationGestureUser : NavigationGestureAuto;

    completed_client_redirect_src_ = Referrer();
  } else if (frame->parent()->isLoading()) {
    document_state->navigation_state()->set_transition_type(
        PAGE_TRANSITION_AUTO_SUBFRAME);
  }

  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_, DidStartProvisionalLoad(frame));

  Send(new ViewHostMsg_DidStartProvisionalLoadForFrame(
       routing_id_, frame->identifier(),
       frame->parent() ? frame->parent()->identifier() : -1,
       is_top_most, ds->request().url()));
}
