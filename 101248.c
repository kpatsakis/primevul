void ScaleRect(float scale, pp::Rect* rect) {
  int left = static_cast<int>(floorf(rect->x() * scale));
  int top = static_cast<int>(floorf(rect->y() * scale));
  int right = static_cast<int>(ceilf((rect->x() + rect->width()) * scale));
  int bottom = static_cast<int>(ceilf((rect->y() + rect->height()) * scale));
  rect->SetRect(left, top, right - left, bottom - top);
}
