  virtual ~WindowObserver() {
    view_->window_->RemoveObserver(this);
  }
