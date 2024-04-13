void RenderWidgetHostImpl::FrameSwapped(const ui::LatencyInfo& latency_info) {
  ui::LatencyInfo::LatencyComponent window_snapshot_component;
  if (latency_info.FindLatency(ui::WINDOW_SNAPSHOT_FRAME_NUMBER_COMPONENT,
                               GetLatencyComponentId(),
                               &window_snapshot_component)) {
    int sequence_number = static_cast<int>(
        window_snapshot_component.sequence_number);
#if defined(OS_MACOSX)
    base::MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&RenderWidgetHostImpl::WindowSnapshotReachedScreen,
                   weak_factory_.GetWeakPtr(),
                   sequence_number),
        base::TimeDelta::FromSecondsD(1. / 6));
#else
    WindowSnapshotReachedScreen(sequence_number);
#endif
  }

  latency_tracker_.OnFrameSwapped(latency_info);
}
