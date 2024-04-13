void Document::SetColorScheme(ColorScheme color_scheme) {
  if (color_scheme_ == color_scheme)
    return;
  color_scheme_ = color_scheme;
  PlatformColorsChanged();
  if (LocalFrameView* view = View()) {
    if (color_scheme == ColorScheme::kDark)
      view->SetBaseBackgroundColor(Color::kBlack);
    else
      view->SetBaseBackgroundColor(Color::kWhite);
  }
}
