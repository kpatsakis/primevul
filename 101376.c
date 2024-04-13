void RenderWidgetHostImpl::OnSyntheticGestureCompleted(
    SyntheticGesture::Result result) {
  Send(new InputMsg_SyntheticGestureCompleted(GetRoutingID()));
}
