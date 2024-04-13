void RenderWidgetHostImpl::ForwardEmulatedGestureEvent(
    const blink::WebGestureEvent& gesture_event) {
  ForwardGestureEvent(gesture_event);
}
