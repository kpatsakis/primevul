void OmniboxViewViews::SetEmphasis(bool emphasize, const gfx::Range& range) {
  SkColor color = location_bar_view_->GetColor(
      emphasize ? LocationBarView::TEXT : LocationBarView::DEEMPHASIZED_TEXT);
  if (range.IsValid())
    ApplyColor(color, range);
  else
    SetColor(color);
}
