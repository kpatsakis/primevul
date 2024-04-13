static inline int imgCoordMungeLowerC(SplashCoord x, GBool glyphMode) {
  return glyphMode ? (splashCeil(x + 0.5) - 1) : splashFloor(x);
}
