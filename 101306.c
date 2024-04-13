void PaintLayerScrollableArea::ScrollbarVisibilityChanged() {
  UpdateScrollbarEnabledState();

  layer_->GetLayoutObject().SetNeedsPaintPropertyUpdate();

  layer_->ClearClipRects();

  if (LayoutView* view = GetLayoutBox()->View())
    view->ClearHitTestCache();
}
