void RenderWidgetHostImpl::SetScreenOrientationForTesting(
    uint16_t angle,
    ScreenOrientationValues type) {
  screen_orientation_angle_for_testing_ = angle;
  screen_orientation_type_for_testing_ = type;
  SynchronizeVisualProperties();
}
