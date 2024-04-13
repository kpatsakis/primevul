void ChildProcessSecurityPolicyImpl::RevokeAllPermissionsForFile(
    int child_id, const FilePath& file) {
  base::AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(child_id);
  if (state == security_state_.end())
    return;

  state->second->RevokeAllPermissionsForFile(file);
}
