void ClientUsageTracker::UpdateUsageCache(
    const GURL& origin, int64 delta) {
  std::string host = net::GetHostOrSpecFromURL(origin);
  if (cached_hosts_.find(host) != cached_hosts_.end()) {
    cached_usage_[host][origin] += delta;
    global_usage_ += delta;
    if (global_unlimited_usage_is_valid_ && IsStorageUnlimited(origin))
      global_unlimited_usage_ += delta;
    DCHECK_GE(cached_usage_[host][origin], 0);
    DCHECK_GE(global_usage_, 0);
    return;
  }

  GetHostUsage(host,
                NewCallback(this, &ClientUsageTracker::NoopHostUsageCallback));
 }
