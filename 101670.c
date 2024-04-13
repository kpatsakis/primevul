void RenderWidgetHostImpl::OnUnexpectedEventAck(UnexpectedEventAckType type) {
  if (type == BAD_ACK_MESSAGE) {
    bad_message::ReceivedBadMessage(process_, bad_message::RWH_BAD_ACK_MESSAGE);
  } else if (type == UNEXPECTED_EVENT_TYPE) {
    suppress_events_until_keydown_ = false;
  }
}
