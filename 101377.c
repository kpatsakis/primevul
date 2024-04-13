void RenderWidgetHostImpl::OnTouchEventAck(
    const TouchEventWithLatencyInfo& event,
    InputEventAckState ack_result) {
  latency_tracker_.OnInputEventAck(event.event, &event.latency);

  if (touch_emulator_ &&
      touch_emulator_->HandleTouchEventAck(event.event, ack_result)) {
    return;
  }

  if (view_)
    view_->ProcessAckedTouchEvent(event, ack_result);
}
