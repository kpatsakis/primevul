void RenderWidgetHostImpl::OnAutoscrollEnd() {
  WebGestureEvent cancel_event = SyntheticWebGestureEventBuilder::Build(
      WebInputEvent::kGestureFlingCancel,
      blink::kWebGestureDeviceSyntheticAutoscroll);
  cancel_event.data.fling_cancel.prevent_boosting = true;
  input_router_->SendGestureEvent(GestureEventWithLatencyInfo(cancel_event));

  WebGestureEvent end_event = SyntheticWebGestureEventBuilder::Build(
      WebInputEvent::kGestureScrollEnd,
      blink::kWebGestureDeviceSyntheticAutoscroll);
  input_router_->SendGestureEvent(GestureEventWithLatencyInfo(end_event));
}
