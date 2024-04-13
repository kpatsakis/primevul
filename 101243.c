void OutOfProcessInstance::Invalidate(const pp::Rect& rect) {
  pp::Rect offset_rect(rect);
  offset_rect.Offset(available_area_.point());
  paint_manager_.InvalidateRect(offset_rect);
}
