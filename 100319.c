bool ChildProcessSecurityPolicy::IsPseudoScheme(const std::string& scheme) {
  AutoLock lock(lock_);

  return (pseudo_schemes_.find(scheme) != pseudo_schemes_.end());
}
