  virtual const Network* active_network() const {
    if (ethernet_ && ethernet_->is_active())
      return ethernet_;
    if (wifi_ && wifi_->is_active())
      return wifi_;
    if (cellular_ && cellular_->is_active())
      return cellular_;
    return NULL;
  }
