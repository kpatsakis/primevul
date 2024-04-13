bool BaseAudioContext::HasPendingActivity() const {
  if (audioWorklet() && audioWorklet()->HasPendingTasks()) {
    return true;
  }

  return !is_cleared_;
}
