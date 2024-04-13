  void CloseDevToolsWindow() {
    Browser* browser = window_->browser();
    content::WindowedNotificationObserver close_observer(
        content::NOTIFICATION_WEB_CONTENTS_DESTROYED,
        content::Source<content::WebContents>(window_->web_contents()));
    browser->tab_strip_model()->CloseAllTabs();
    close_observer.Wait();
  }
