MainThreadFrameObserver::MainThreadFrameObserver(
    RenderWidgetHost* render_widget_host)
    : render_widget_host_(render_widget_host),
      routing_id_(render_widget_host_->GetProcess()->GetNextRoutingID()) {
  render_widget_host_->GetProcess()->AddRoute(routing_id_, this);
}
