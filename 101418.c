void CustomButton::ViewHierarchyChanged(
    const ViewHierarchyChangedDetails& details) {
  if (!details.is_add && state_ != STATE_DISABLED)
    SetState(STATE_NORMAL);
}
