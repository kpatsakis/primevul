void SimulateTapAt(WebContents* web_contents, const gfx::Point& point) {
  blink::WebGestureEvent tap(blink::WebGestureEvent::kGestureTap, 0,
                             ui::EventTimeForNow(),
                             blink::kWebGestureDeviceTouchscreen);
  tap.SetPositionInWidget(gfx::PointF(point));
  RenderWidgetHostImpl* widget_host = RenderWidgetHostImpl::From(
      web_contents->GetRenderViewHost()->GetWidget());
  widget_host->ForwardGestureEvent(tap);
}
