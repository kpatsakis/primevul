bool DevToolsWindow::PreHandleKeyboardEvent(
    content::WebContents* source,
    const content::NativeWebKeyboardEvent& event,
    bool* is_keyboard_shortcut) {
  if (IsDocked()) {
    BrowserWindow* inspected_window = GetInspectedBrowserWindow();
    if (inspected_window) {
      return inspected_window->PreHandleKeyboardEvent(event,
                                                      is_keyboard_shortcut);
    }
  }
  return false;
}
