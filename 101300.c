GraphicsLayer* PaintLayerScrollableArea::LayerForScrolling() const {
  return Layer()->HasCompositedLayerMapping()
             ? Layer()->GetCompositedLayerMapping()->ScrollingContentsLayer()
             : nullptr;
}
