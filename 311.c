UsageTracker::~UsageTracker() {
  STLDeleteValues(&client_tracker_map_);
}
