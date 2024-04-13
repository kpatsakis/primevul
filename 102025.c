void RenderWidgetHostImpl::RestartHangMonitorTimeoutIfNecessary() {
  if (hang_monitor_timeout_ && in_flight_event_count_ > 0 && !is_hidden_)
    hang_monitor_timeout_->Restart(hung_renderer_delay_);
}
