bool CustomButton::OnKeyPressed(const ui::KeyEvent& event) {
  if (state_ == STATE_DISABLED)
    return false;

  if (event.key_code() == ui::VKEY_SPACE) {
    SetState(STATE_PRESSED);
  } else if (event.key_code() == ui::VKEY_RETURN) {
    SetState(STATE_NORMAL);
    NotifyClick(event);
  } else {
    return false;
  }
  return true;
}
