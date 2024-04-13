void Browser::UpdateUIForNavigationInTab(
    WebContents* contents,
    ui::PageTransition transition,
    chrome::NavigateParams::WindowAction action,
    bool user_initiated) {
  tab_strip_model_->TabNavigating(contents, transition);

  bool contents_is_selected =
      contents == tab_strip_model_->GetActiveWebContents();
  if (user_initiated && contents_is_selected && window()->GetLocationBar()) {
    window()->GetLocationBar()->Revert();
  }

  if (GetStatusBubble())
    GetStatusBubble()->Hide();

  ScheduleUIUpdate(contents, content::INVALIDATE_TYPE_URL);

  if (contents_is_selected &&
      (window()->IsActive() || action == chrome::NavigateParams::SHOW_WINDOW)) {
    contents->SetInitialFocus();
  }
}
