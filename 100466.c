  const int8* GetPixelAddress(GLint x, GLint y) const {
    return pixels_ + (width_ * y + x) * bytes_per_pixel_;
  }
