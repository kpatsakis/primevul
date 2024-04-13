bool Document::NeedsLayoutTreeUpdate() const {
  if (!IsActive() || !View())
    return false;
  if (NeedsFullLayoutTreeUpdate())
    return true;
  if (ChildNeedsStyleRecalc())
    return true;
  if (ChildNeedsStyleInvalidation())
    return true;
  if (ChildNeedsReattachLayoutTree()) {
    DCHECK(InStyleRecalc());
    return true;
  }
  if (GetLayoutView() && GetLayoutView()->WasNotifiedOfSubtreeChange())
    return true;
  return false;
}
