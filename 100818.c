DevToolsWindow* DevToolsWindow::GetInstanceForInspectedRenderViewHost(
      content::RenderViewHost* inspected_rvh) {
  if (!inspected_rvh || !DevToolsAgentHost::HasFor(inspected_rvh))
    return NULL;

  scoped_refptr<DevToolsAgentHost> agent(DevToolsAgentHost::GetOrCreateFor(
      inspected_rvh));
  return FindDevToolsWindow(agent.get());
}
