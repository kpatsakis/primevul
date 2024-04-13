  virtual bool cellular_connected() const {
    return cellular_ ? cellular_->connected() : false;
  }
