void DevToolsWindow::InspectElement(content::RenderViewHost* inspected_rvh,
                                    int x,
                                    int y) {
  scoped_refptr<DevToolsAgentHost> agent(
      DevToolsAgentHost::GetOrCreateFor(inspected_rvh));
  agent->InspectElement(x, y);
  OpenDevToolsWindow(inspected_rvh);
}
