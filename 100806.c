  void ToggleDevToolsWindow() {
    content::WindowedNotificationObserver close_observer(
        content::NOTIFICATION_WEB_CONTENTS_DESTROYED,
        content::Source<content::WebContents>(window_->web_contents()));
    DevToolsWindow::ToggleDevToolsWindow(inspected_rvh_, false,
        DevToolsToggleAction::Toggle());
    close_observer.Wait();
  }
