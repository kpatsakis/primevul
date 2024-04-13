  void IncrementRefCount(const std::string& uuid) {
    context_->IncrementBlobRefCount(uuid);
  }
