void OmniboxViewViews::ShowVirtualKeyboardIfEnabled() {
  if (auto* input_method = GetInputMethod())
    input_method->ShowVirtualKeyboardIfEnabled();
}
