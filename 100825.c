void DevToolsWindow::Show(const DevToolsToggleAction& action) {
  if (IsDocked()) {
    Browser* inspected_browser = NULL;
    int inspected_tab_index = -1;
    if (!IsInspectedBrowserPopup() &&
        FindInspectedBrowserAndTabIndex(&inspected_browser,
                                        &inspected_tab_index)) {
      BrowserWindow* inspected_window = inspected_browser->window();
      web_contents_->SetDelegate(this);
      inspected_window->UpdateDevTools();
      web_contents_->GetView()->SetInitialFocus();
      inspected_window->Show();
      TabStripModel* tab_strip_model = inspected_browser->tab_strip_model();
      tab_strip_model->ActivateTabAt(inspected_tab_index, true);
      PrefsTabHelper::CreateForWebContents(web_contents_);
      GetRenderViewHost()->SyncRendererPrefs();
      ScheduleAction(action);
      return;
    }

    dock_side_ = DEVTOOLS_DOCK_SIDE_UNDOCKED;
  }

  bool should_show_window =
      !browser_ || (action.type() != DevToolsToggleAction::kInspect);

  if (!browser_)
    CreateDevToolsBrowser();

  if (should_show_window) {
    browser_->window()->Show();
    web_contents_->GetView()->SetInitialFocus();
  }

  ScheduleAction(action);
}
