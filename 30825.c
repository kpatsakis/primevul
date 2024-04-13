static inline int imgCoordMungeUpper(SplashCoord x) {
  return splashCeil(x + 0.5) - 1;
}
