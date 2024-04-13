void RenderWidgetHostViewAura::Hide() {
  window_->Hide();
  WasHidden();
}
