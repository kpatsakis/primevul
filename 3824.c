  int64_t CardinalityInternal() const override {
    return sparse_tensor_.shape()[0];
  }