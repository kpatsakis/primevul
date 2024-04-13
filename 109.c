  virtual bool cellular_connecting() const {
    return cellular_ ? cellular_->connecting() : false;
  }
