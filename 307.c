ClientUsageTracker::~ClientUsageTracker() {
  if (special_storage_policy_)
    special_storage_policy_->RemoveObserver(this);
}
