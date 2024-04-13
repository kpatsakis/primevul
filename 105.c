  virtual bool cellular_available() const {
    return available_devices_ & (1 << TYPE_CELLULAR);
  }
