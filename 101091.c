bool QueryManager::ProcessPendingTransferQueries() {
  while (!pending_transfer_queries_.empty()) {
    Query* query = pending_transfer_queries_.front().get();
    if (!query->Process()) {
      return false;
    }
    if (query->pending()) {
      break;
    }
    query->RunCallbacks();
    pending_transfer_queries_.pop_front();
  }

  return true;
}
