IntPoint PaintLayerScrollableArea::ConvertFromRootFrame(
    const IntPoint& point_in_root_frame) const {
  LayoutView* view = GetLayoutBox()->View();
  if (!view)
    return point_in_root_frame;

  return view->GetFrameView()->ConvertFromRootFrame(point_in_root_frame);
}
