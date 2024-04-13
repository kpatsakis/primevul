bool PaintLayerScrollableArea::HitTestOverflowControls(
    HitTestResult& result,
    const IntPoint& local_point) {
  if (!HasScrollbar() && !GetLayoutBox()->CanResize())
    return false;

  IntRect resize_control_rect;
  if (GetLayoutBox()->StyleRef().Resize() != EResize::kNone) {
    resize_control_rect =
        ResizerCornerRect(GetLayoutBox()->PixelSnappedBorderBoxRect(
                              Layer()->SubpixelAccumulation()),
                          kResizerForPointer);
    if (resize_control_rect.Contains(local_point))
      return true;
  }
  int resize_control_size = max(resize_control_rect.Height(), 0);

  IntRect visible_rect = VisibleContentRect(kIncludeScrollbars);

  if (HasVerticalScrollbar() &&
      VerticalScrollbar()->ShouldParticipateInHitTesting()) {
    LayoutRect v_bar_rect(
        VerticalScrollbarStart(0, Layer()->PixelSnappedSize().Width()),
        GetLayoutBox()->BorderTop().ToInt(),
        VerticalScrollbar()->ScrollbarThickness(),
        visible_rect.Height() -
            (HasHorizontalScrollbar()
                 ? HorizontalScrollbar()->ScrollbarThickness()
                 : resize_control_size));
    if (v_bar_rect.Contains(local_point)) {
      result.SetScrollbar(VerticalScrollbar());
      return true;
    }
  }

  resize_control_size = max(resize_control_rect.Width(), 0);
  if (HasHorizontalScrollbar() &&
      HorizontalScrollbar()->ShouldParticipateInHitTesting()) {
    int h_scrollbar_thickness = HorizontalScrollbar()->ScrollbarThickness();
    LayoutRect h_bar_rect(
        HorizontalScrollbarStart(0),
        GetLayoutBox()->BorderTop().ToInt() + visible_rect.Height() -
            h_scrollbar_thickness,
        visible_rect.Width() - (HasVerticalScrollbar()
                                    ? VerticalScrollbar()->ScrollbarThickness()
                                    : resize_control_size),
        h_scrollbar_thickness);
    if (h_bar_rect.Contains(local_point)) {
      result.SetScrollbar(HorizontalScrollbar());
      return true;
    }
  }


  return false;
}
