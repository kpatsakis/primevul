void BrowserCommandController::UpdateCommandsForHostedAppAvailability() {
  bool has_toolbar =
      browser_->is_type_tabbed() ||
      extensions::HostedAppBrowserController::IsForExperimentalHostedAppBrowser(
          browser_);
  if (window() && window()->ShouldHideUIForFullscreen())
    has_toolbar = false;
  command_updater_.UpdateCommandEnabled(IDC_FOCUS_TOOLBAR, has_toolbar);
  command_updater_.UpdateCommandEnabled(IDC_FOCUS_NEXT_PANE, has_toolbar);
  command_updater_.UpdateCommandEnabled(IDC_FOCUS_PREVIOUS_PANE, has_toolbar);
  command_updater_.UpdateCommandEnabled(IDC_SHOW_APP_MENU, has_toolbar);
}
