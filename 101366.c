void RenderWidgetHostImpl::ForwardTouchEventWithLatencyInfo(
      const blink::WebTouchEvent& touch_event,
      const ui::LatencyInfo& ui_latency) {
  TRACE_EVENT0("input", "RenderWidgetHostImpl::ForwardTouchEvent");


  TouchEventWithLatencyInfo touch_with_latency(touch_event, ui_latency);
  if (touch_emulator_ &&
      touch_emulator_->HandleTouchEvent(touch_with_latency.event)) {
    if (view_) {
      view_->ProcessAckedTouchEvent(
          touch_with_latency, INPUT_EVENT_ACK_STATE_CONSUMED);
    }
    return;
  }

  latency_tracker_.OnInputEvent(touch_event, &touch_with_latency.latency);
  input_router_->SendTouchEvent(touch_with_latency);
}
