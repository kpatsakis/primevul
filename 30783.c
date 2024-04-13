static inline int imgCoordMungeUpperC(SplashCoord x, GBool glyphMode) {
  return glyphMode ? (splashCeil(x + 0.5) - 1) : (splashFloor(x) + 1);
}
