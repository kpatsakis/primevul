void Browser::TabDeactivated(WebContents* contents) {
  exclusive_access_manager_->OnTabDeactivated(contents);
  SearchTabHelper::FromWebContents(contents)->OnTabDeactivated();

  window_->GetLocationBar()->SaveStateToContents(contents);
}
