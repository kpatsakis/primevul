void RenderViewImpl::OnMoveCaret(const gfx::Point& point) {
  if (!webview())
    return;

  Send(new ViewHostMsg_MoveCaret_ACK(routing_id_));

  webview()->focusedFrame()->moveCaretSelectionTowardsWindowPoint(point);
}
