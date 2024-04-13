void BrowserNonClientFrameViewAura::ButtonPressed(views::Button* sender,
                                                  const views::Event& event) {
  if (sender == maximize_button_) {
    ResetWindowControls();
    if (frame()->IsMaximized())
      frame()->Restore();
    else
      frame()->Maximize();
  } else if (sender == close_button_) {
    frame()->Close();
  }
}
