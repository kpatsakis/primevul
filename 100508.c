bool RootWindow::DispatchTouchEvent(TouchEvent* event) {
  DispatchHeldMouseMove();
  if (ui::IsDIPEnabled()) {
    float scale = ui::GetDeviceScaleFactor(layer());
    ui::Transform transform = layer()->transform();
    transform.ConcatScale(scale, scale);
    event->UpdateForRootTransform(transform);
  } else {
    event->UpdateForRootTransform(layer()->transform());
  }
  bool handled = false;
  ui::TouchStatus status = ui::TOUCH_STATUS_UNKNOWN;
  Window* target = capture_window_;
  if (!target) {
    target = ConsumerToWindow(
        gesture_recognizer_->GetTouchLockedTarget(event));
    if (!target) {
      target = ConsumerToWindow(
          gesture_recognizer_->GetTargetForLocation(event->GetLocation()));
    }
  }

  if (!target && !bounds().Contains(event->location())) {
    target = this;
  } else {
    if (!target) {
      target = GetEventHandlerForPoint(event->location());
      if (!target)
        return false;
    }

    TouchEvent translated_event(*event, this, target);
    status = ProcessTouchEvent(target, &translated_event);
    handled = status != ui::TOUCH_STATUS_UNKNOWN;

    if (status == ui::TOUCH_STATUS_QUEUED ||
        status == ui::TOUCH_STATUS_QUEUED_END)
      gesture_recognizer_->QueueTouchEventForGesture(target, *event);
  }

  scoped_ptr<ui::GestureRecognizer::Gestures> gestures;
  gestures.reset(gesture_recognizer_->ProcessTouchEventForGesture(
      *event, status, target));

  return ProcessGestures(gestures.get()) ? true : handled;
}
