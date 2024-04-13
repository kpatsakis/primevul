bool ChildProcessSecurityPolicy::CanReadRawCookies(int renderer_id) {
  AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(renderer_id);
  if (state == security_state_.end())
    return false;

  return state->second->can_read_raw_cookies();
}
