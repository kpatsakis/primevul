static inline Guchar clip255(int x) {
  return x < 0 ? 0 : x > 255 ? 255 : x;
}
