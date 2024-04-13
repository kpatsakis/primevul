pp::Rect PDFiumEngine::GetScreenRect(const pp::Rect& rect) const {
  pp::Rect rv;
  int right =
      static_cast<int>(ceil(rect.right() * current_zoom_ - position_.x()));
  int bottom =
    static_cast<int>(ceil(rect.bottom() * current_zoom_ - position_.y()));

  rv.set_x(static_cast<int>(rect.x() * current_zoom_ - position_.x()));
  rv.set_y(static_cast<int>(rect.y() * current_zoom_ - position_.y()));
  rv.set_width(right - rv.x());
  rv.set_height(bottom - rv.y());
  return rv;
}
