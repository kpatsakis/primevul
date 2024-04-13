void RootWindow::OnHostResized(const gfx::Size& size_in_pixel) {
  DispatchHeldMouseMove();
  compositor_->SetScaleAndSize(GetDeviceScaleFactorFromMonitor(this),
                               size_in_pixel);
  gfx::Size old(bounds().size());
  gfx::Rect bounds(ui::ConvertSizeToDIP(layer(), size_in_pixel));
  layer()->transform().TransformRect(&bounds);
  SetBounds(bounds);
  FOR_EACH_OBSERVER(RootWindowObserver, observers_,
                    OnRootWindowResized(this, old));
}
