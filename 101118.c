void RenderWidgetHostViewAura::SetSize(const gfx::Size& size) {
  InternalSetBounds(gfx::Rect(window_->bounds().origin(), size));
}
