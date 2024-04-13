void RenderWidgetHostImpl::NotifyScreenInfoChanged() {
  color_profile_out_of_date_ = true;

  if (delegate_)
    delegate_->ScreenInfoChanged();

  WasResized();
}
