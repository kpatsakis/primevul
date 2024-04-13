void RenderWidgetHostImpl::ProgressFlingIfNeeded(TimeTicks current_time) {
  browser_fling_needs_begin_frame_ = false;
  fling_scheduler_->ProgressFlingOnBeginFrameIfneeded(current_time);
}
