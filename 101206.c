void RenderViewTest::SimulatePointClick(const gfx::Point& point) {
  WebMouseEvent mouse_event(WebInputEvent::kMouseDown,
                            WebInputEvent::kNoModifiers,
                            ui::EventTimeStampToSeconds(ui::EventTimeForNow()));
  mouse_event.button = WebMouseEvent::Button::kLeft;
  mouse_event.SetPositionInWidget(point.x(), point.y());
  mouse_event.click_count = 1;
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->OnMessageReceived(InputMsg_HandleInputEvent(
      0, &mouse_event, std::vector<const WebInputEvent*>(), ui::LatencyInfo(),
      InputEventDispatchType::DISPATCH_TYPE_BLOCKING));
  mouse_event.SetType(WebInputEvent::kMouseUp);
  impl->OnMessageReceived(InputMsg_HandleInputEvent(
      0, &mouse_event, std::vector<const WebInputEvent*>(), ui::LatencyInfo(),
      InputEventDispatchType::DISPATCH_TYPE_BLOCKING));
}
