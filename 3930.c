    size_t operator()(const ArrayOrObject data) const {
      return data.toOpaque();
    }