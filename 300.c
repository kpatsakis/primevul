void ClientUsageTracker::OnSpecialStoragePolicyChanged() {
  DCHECK(CalledOnValidThread());
  global_unlimited_usage_is_valid_ = false;
}
