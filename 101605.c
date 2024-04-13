void ExtensionDevToolsClientHost::Close() {
  agent_host_->DetachClient(this);
  delete this;
}
