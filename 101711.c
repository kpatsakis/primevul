void PDFiumEngine::OnSelectionChanged() {
  DCHECK(!in_form_text_area_);
  pp::PDF::SetSelectedText(GetPluginInstance(), GetSelectedText().c_str());

  pp::Rect left(std::numeric_limits<int32_t>::max(),
                std::numeric_limits<int32_t>::max(), 0, 0);
  pp::Rect right;
  for (auto& sel : selection_) {
    const std::vector<pp::Rect>& screen_rects = sel.GetScreenRects(
        GetVisibleRect().point(), current_zoom_, current_rotation_);
    for (const auto& rect : screen_rects) {
      if (IsAboveOrDirectlyLeftOf(rect, left))
        left = rect;
      if (IsAboveOrDirectlyLeftOf(right, rect))
        right = rect;
    }
  }
  right.set_x(right.x() + right.width());
  if (left.IsEmpty()) {
    left.set_x(0);
    left.set_y(0);
  }
  client_->SelectionChanged(left, right);
}
