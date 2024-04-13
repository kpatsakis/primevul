void Splash::strokeWide(SplashPath *path, SplashCoord w) {
  SplashPath *path2;

  path2 = makeStrokePath(path, w, gFalse);
  fillWithPattern(path2, gFalse, state->strokePattern, state->strokeAlpha);
  delete path2;
}
