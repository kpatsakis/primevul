void RenderWidgetHostImpl::OnAutoscrollStart(const gfx::PointF& position) {
  WebGestureEvent scroll_begin = SyntheticWebGestureEventBuilder::Build(
      WebInputEvent::kGestureScrollBegin,
      blink::kWebGestureDeviceSyntheticAutoscroll);

  scroll_begin.x = position.x();
  scroll_begin.y = position.y();
  scroll_begin.source_device = blink::kWebGestureDeviceSyntheticAutoscroll;

  input_router_->SendGestureEvent(GestureEventWithLatencyInfo(scroll_begin));
}
