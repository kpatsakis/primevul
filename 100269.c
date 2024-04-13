void RenderView::UserMetricsRecordAction(const std::string& action) {
  Send(new ViewHostMsg_UserMetricsRecordAction(routing_id_, action));
}
