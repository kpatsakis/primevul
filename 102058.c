void SimulateGestureEvent(WebContents* web_contents,
                          const blink::WebGestureEvent& gesture_event,
                          const ui::LatencyInfo& latency) {
  RenderWidgetHostViewBase* view = static_cast<RenderWidgetHostViewBase*>(
      web_contents->GetRenderWidgetHostView());
  view->ProcessGestureEvent(gesture_event, latency);
}
