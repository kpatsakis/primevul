UsageTracker::UsageTracker(const QuotaClientList& clients, StorageType type,
                           SpecialStoragePolicy* special_storage_policy)
    : type_(type),
      callback_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)) {
  for (QuotaClientList::const_iterator iter = clients.begin();
      iter != clients.end();
      ++iter) {
    client_tracker_map_.insert(std::make_pair(
        (*iter)->id(),
        new ClientUsageTracker(this, *iter, type, special_storage_policy)));
  }
}
