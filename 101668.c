void RenderWidgetHostImpl::OnMessageDispatchError(const IPC::Message& message) {
  RenderProcessHost* rph = GetProcess();
  rph->OnBadMessageReceived(message);
}
