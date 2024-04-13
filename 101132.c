void OverscrollControllerAndroid::DidToggleFullscreenModeForTab(
    bool entered_fullscreen) {
  DCHECK(refresh_effect_);
  if (is_fullscreen_ == entered_fullscreen)
    return;
  is_fullscreen_ = entered_fullscreen;
  if (is_fullscreen_)
    refresh_effect_->ReleaseWithoutActivation();
}
