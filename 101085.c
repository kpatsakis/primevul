void QueryManager::Query::AddCallback(base::Closure callback) {
  if (pending_) {
    callbacks_.push_back(callback);
  } else {
    callback.Run();
  }
}
