void ChildProcessSecurityPolicy::Add(int renderer_id) {
  AutoLock lock(lock_);
  if (security_state_.count(renderer_id) != 0) {
    NOTREACHED() << "Add renderers at most once.";
    return;
  }

  security_state_[renderer_id] = new SecurityState();
}
