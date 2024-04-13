bool RootWindow::DispatchMouseEventImpl(MouseEvent* event) {
  if (ui::IsDIPEnabled()) {
    float scale = ui::GetDeviceScaleFactor(layer());
    ui::Transform transform = layer()->transform();
    transform.ConcatScale(scale, scale);
    event->UpdateForRootTransform(transform);
  } else {
    event->UpdateForRootTransform(layer()->transform());
  }
  Window* target =
      mouse_pressed_handler_ ? mouse_pressed_handler_ : capture_window_;
  if (!target)
    target = GetEventHandlerForPoint(event->location());
  return DispatchMouseEventToTarget(event, target);
}
