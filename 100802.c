  void CloseDockedDevTools() {
    DevToolsWindow::ToggleDevToolsWindow(inspected_rvh_, false,
        DevToolsToggleAction::Toggle());
  }
