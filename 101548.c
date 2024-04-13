void OmniboxViewViews::OnBeforePossibleChange() {
  GetState(&state_before_change_);
  ime_composing_before_change_ = IsIMEComposing();

  ClearAccessibilityLabel();
}
