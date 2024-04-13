void RenderWidgetHostImpl::WaitForSurface() {
  const int kPaintMsgTimeoutMS = 50;

  if (!view_)
    return;

  gfx::Size view_size = current_size_;
  if (!auto_resize_enabled_) {
    gfx::Rect view_rect = view_->GetViewBounds();
    if (view_rect.IsEmpty())
      return;
    view_size = view_rect.size();
  }

  TRACE_EVENT2("renderer_host",
               "RenderWidgetHostImpl::WaitForSurface",
               "width",
               base::IntToString(view_size.width()),
               "height",
               base::IntToString(view_size.height()));

  DCHECK(!is_hidden_) << "WaitForSurface called while hidden!";

  DCHECK(!in_get_backing_store_) << "WaitForSurface called recursively!";
  base::AutoReset<bool> auto_reset_in_get_backing_store(
      &in_get_backing_store_, true);

  if (view_->HasAcceleratedSurface(view_size))
    return;

  if (!repaint_ack_pending_ && !resize_ack_pending_) {
    repaint_start_time_ = TimeTicks::Now();
    repaint_ack_pending_ = true;
    TRACE_EVENT_ASYNC_BEGIN0(
        "renderer_host", "RenderWidgetHostImpl::repaint_ack_pending_", this);
    Send(new ViewMsg_Repaint(routing_id_, view_size));
  }

  TimeTicks start_time = TimeTicks::Now();
  TimeDelta time_left = TimeDelta::FromMilliseconds(kPaintMsgTimeoutMS);
  TimeTicks timeout_time = start_time + time_left;
  while (1) {
    TRACE_EVENT0("renderer_host", "WaitForSurface::WaitForSingleTaskToRun");
    if (ui::WindowResizeHelperMac::Get()->WaitForSingleTaskToRun(time_left)) {
      if (auto_resize_enabled_)
        view_size = current_size_;
      if (view_->HasAcceleratedSurface(view_size))
        break;
    }
    time_left = timeout_time - TimeTicks::Now();
    if (time_left <= TimeDelta::FromSeconds(0)) {
      TRACE_EVENT0("renderer_host", "WaitForSurface::Timeout");
      break;
    }
  }
}
