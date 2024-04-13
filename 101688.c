  void SimulateWheelEvent(float dX,
                          float dY,
                          int modifiers,
                          bool precise,
                          WebMouseWheelEvent::Phase phase) {
    WebMouseWheelEvent wheel_event = SyntheticWebMouseWheelEventBuilder::Build(
        0, 0, dX, dY, modifiers, precise);
    wheel_event.phase = phase;
    widget_host_->ForwardWheelEvent(wheel_event);
    base::RunLoop().RunUntilIdle();
  }
