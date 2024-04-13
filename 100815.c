void DevToolsWindow::DocumentOnLoadCompletedInMainFrame() {
  is_loaded_ = true;
  UpdateTheme();
  DoAction();
  AddDevToolsExtensionsToClient();
}
