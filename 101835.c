void RenderProcessHostImpl::OnUserMetricsRecordAction(
    const std::string& action) {
  base::RecordComputedAction(action);
}
