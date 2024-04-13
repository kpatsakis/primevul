int DevToolsWindow::GetWidth(int container_width) {
  if (width_ == -1) {
    width_ = profile_->GetPrefs()->
        GetInteger(prefs::kDevToolsVSplitLocation);
  }

  if (width_ == -1)
    width_ = container_width / 3;

  width_ = std::max(GetMinimumWidth(), width_);

  width_ = std::min(container_width - kMinContentsSize, width_);
  return width_;
}
