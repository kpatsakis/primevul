bool DevToolsWindow::PreHandleGestureEvent(
    WebContents* source,
    const blink::WebGestureEvent& event) {
  return blink::WebInputEvent::IsPinchGestureEventType(event.GetType());
}
