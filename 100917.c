  void CloseDevToolsWindow() {
    DevToolsManager* devtools_manager = DevToolsManager::GetInstance();
    Browser* browser = window_->browser();
    scoped_refptr<DevToolsAgentHost> agent(
        DevToolsAgentHost::GetFor(inspected_rvh_));
    devtools_manager->UnregisterDevToolsClientHostFor(agent);

    if (browser)
      BrowserClosedObserver close_observer(browser);
  }
