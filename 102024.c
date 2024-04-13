void RenderWidgetHostImpl::RendererExited(base::TerminationStatus status,
                                          int exit_code) {
  if (!renderer_initialized_)
    return;

  monitoring_composition_info_ = false;

  renderer_initialized_ = false;

  waiting_for_screen_rects_ack_ = false;

  suppress_events_until_keydown_ = false;

  ResetSizeAndRepaintPendingFlags();
  current_size_.SetSize(0, 0);
  if (!is_hidden_) {
    process_->WidgetHidden();
    is_hidden_ = true;
  }

  in_flight_event_count_ = 0;
  StopHangMonitorTimeout();

  if (view_) {
    view_->RenderProcessGone(status, exit_code);
    view_.reset();  // The View should be deleted by RenderProcessGone.
  }

  SetupInputRouter();
  synthetic_gesture_controller_.reset();

  current_content_source_id_ = 0;

  frame_token_message_queue_->Reset();
}
