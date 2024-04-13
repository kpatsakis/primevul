void Document::RemoveLockedDisplayLock() {
  DCHECK_GT(locked_display_lock_count_, 0);
  --locked_display_lock_count_;
}
