  void ResetMaxCapacityBytes(size_t max_capacity_bytes) {
    max_capacity_bytes_ = max_capacity_bytes;
    Initialize();
  }
