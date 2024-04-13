void DevToolsUIBindings::InnerAttach() {
  DCHECK(agent_host_.get());
  agent_host_->AttachClient(this);
}
