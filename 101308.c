void PaintLayerScrollableArea::UpdateScrollOrigin() {
  if (OverflowRect().IsEmpty())
    return;
  LayoutRect scrollable_overflow(overflow_rect_);
  scrollable_overflow.Move(-GetLayoutBox()->BorderLeft(),
                           -GetLayoutBox()->BorderTop());
  IntPoint new_origin(FlooredIntPoint(-scrollable_overflow.Location()) +
                      GetLayoutBox()->OriginAdjustmentForScrollbars());
  if (new_origin != scroll_origin_)
    scroll_origin_changed_ = true;
  scroll_origin_ = new_origin;
}
