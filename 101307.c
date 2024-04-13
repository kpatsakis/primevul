void PaintLayerScrollableArea::SetHorizontalScrollbarVisualRect(
    const LayoutRect& rect) {
  horizontal_scrollbar_visual_rect_ = rect;
  if (Scrollbar* scrollbar = HorizontalScrollbar())
    scrollbar->SetVisualRect(rect);
}
