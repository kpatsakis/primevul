void RenderWidgetHostImpl::GotResponseToKeyboardLockRequest(bool allowed) {
  DCHECK(keyboard_lock_requested_);
  keyboard_lock_allowed_ = allowed;

  if (keyboard_lock_allowed_)
    LockKeyboard();
  else
    UnlockKeyboard();
}
