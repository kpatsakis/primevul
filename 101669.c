void RenderWidgetHostImpl::OnProcessSwapMessage(const IPC::Message& message) {
  RenderProcessHost* rph = GetProcess();
  rph->OnMessageReceived(message);
}
