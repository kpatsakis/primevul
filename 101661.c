void RenderWidgetHostImpl::ForwardEmulatedTouchEvent(
    const blink::WebTouchEvent& touch_event,
    RenderWidgetHostViewBase* target) {
  TRACE_EVENT0("input", "RenderWidgetHostImpl::ForwardEmulatedTouchEvent");
  ForwardTouchEventWithLatencyInfo(touch_event,
                                   ui::LatencyInfo(ui::SourceEventType::TOUCH));
}
