void OmniboxViewViews::OnCompositingEnded(ui::Compositor* compositor) {
  if (latency_histogram_state_ == COMPOSITING_STARTED) {
    DCHECK(!insert_char_time_.is_null());
    UMA_HISTOGRAM_TIMES("Omnibox.CharTypedToRepaintLatency",
                        base::TimeTicks::Now() - insert_char_time_);
    insert_char_time_ = base::TimeTicks();
    latency_histogram_state_ = NOT_ACTIVE;
  }
}
