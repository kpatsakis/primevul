void QueryManager::StopTracking(QueryManager::Query* /* query */) {
  --query_count_;
}
