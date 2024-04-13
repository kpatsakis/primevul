inline blink::WebGestureEvent CreateScrollEndForWrapping(
    const blink::WebGestureEvent& gesture_event) {
  DCHECK(gesture_event.GetType() == blink::WebInputEvent::kGestureScrollUpdate);

  blink::WebGestureEvent wrap_gesture_scroll_end(
      blink::WebInputEvent::kGestureScrollEnd, gesture_event.GetModifiers(),
      gesture_event.TimeStamp(), gesture_event.SourceDevice());
  wrap_gesture_scroll_end.resending_plugin_id =
      gesture_event.resending_plugin_id;
  wrap_gesture_scroll_end.data.scroll_end.delta_units =
      gesture_event.data.scroll_update.delta_units;

  return wrap_gesture_scroll_end;
}
