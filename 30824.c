static inline int imgCoordMungeLower(SplashCoord x) {
  return splashCeil(x + 0.5) - 1;
}
