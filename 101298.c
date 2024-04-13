void PaintLayerScrollableArea::GetTickmarks(Vector<IntRect>& tickmarks) const {
  if (layer_->IsRootLayer())
    tickmarks = ToLayoutView(GetLayoutBox())->GetTickmarks();
}
