  std::string ConvertHostPointToScreen(int x, int y) const {
    gfx::Point point(x, y);
    GetScreenPositionController()->ConvertHostPointToScreen(
        window_->GetRootWindow(), &point);
    return point.ToString();
  }
