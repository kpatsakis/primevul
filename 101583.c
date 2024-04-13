WebHistoryItem HistoryController::GetItemForNewChildFrame(
    RenderFrameImpl* frame) const {
  if (navigation_params_.get()) {
    frame->SetPendingNavigationParams(make_scoped_ptr(
        new NavigationParams(*navigation_params_.get())));
  }

  if (!current_entry_)
    return WebHistoryItem();
  return current_entry_->GetItemForFrame(frame);
}
