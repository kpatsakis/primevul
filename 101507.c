bool Browser::ShouldHideUIForFullscreen() const {
  return window_ && window_->ShouldHideUIForFullscreen();
}
