  ~AudioSourceProviderClientLockScope() {
    if (client_)
      client_->unlock();
  }
