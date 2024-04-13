void RenderWidgetHostViewAura::OnCompositingEnded(
    ui::Compositor* compositor) {
  if (paint_observer_)
    paint_observer_->OnCompositingComplete();
}
