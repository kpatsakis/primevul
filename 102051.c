InputMsgWatcher::InputMsgWatcher(RenderWidgetHost* render_widget_host,
                                 blink::WebInputEvent::Type type)
    : render_widget_host_(render_widget_host),
      wait_for_type_(type),
      ack_result_(INPUT_EVENT_ACK_STATE_UNKNOWN),
      ack_source_(InputEventAckSource::UNKNOWN) {
  render_widget_host->AddInputEventObserver(this);
}
