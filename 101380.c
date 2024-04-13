void RenderWidgetHostImpl::SendCursorVisibilityState(bool is_visible) {
  Send(new InputMsg_CursorVisibilityChange(GetRoutingID(), is_visible));
}
