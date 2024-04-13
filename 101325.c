  virtual ~CallbackOwner() {
    *deleted_ = true;
  }
