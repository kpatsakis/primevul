void RenderWidgetHostViewAura::UpdateConstrainedWindowRects(
    const std::vector<gfx::Rect>& rects) {
  if (rects == constrained_rects_)
    return;
  constrained_rects_ = rects;
  UpdateCutoutRects();
}
