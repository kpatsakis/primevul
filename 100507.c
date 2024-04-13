bool RootWindow::DispatchScrollEvent(ScrollEvent* event) {
  DispatchHeldMouseMove();
  if (ui::IsDIPEnabled()) {
    float scale = ui::GetDeviceScaleFactor(layer());
    ui::Transform transform = layer()->transform();
    transform.ConcatScale(scale, scale);
    event->UpdateForRootTransform(transform);
  } else {
    event->UpdateForRootTransform(layer()->transform());
  }

  last_mouse_location_ = event->location();
  synthesize_mouse_move_ = false;

  Window* target =
      mouse_pressed_handler_ ? mouse_pressed_handler_ : capture_window_;
  if (!target)
    target = GetEventHandlerForPoint(event->location());

  if (target && target->delegate()) {
    int flags = event->flags();
    gfx::Point location_in_window = event->location();
    Window::ConvertPointToWindow(this, target, &location_in_window);
    if (IsNonClientLocation(target, location_in_window))
      flags |= ui::EF_IS_NON_CLIENT;
    ScrollEvent translated_event(*event, this, target, event->type(), flags);
    return ProcessMouseEvent(target, &translated_event);
  }
  return false;
}
