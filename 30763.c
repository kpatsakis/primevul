Splash::Splash(SplashBitmap *bitmapA, GBool vectorAntialiasA,
	       SplashScreenParams *screenParams) {
  int i;

  bitmap = bitmapA;
  vectorAntialias = vectorAntialiasA;
  inShading = gFalse;
  state = new SplashState(bitmap->width, bitmap->height, vectorAntialias,
			  screenParams);
  if (vectorAntialias) {
    aaBuf = new SplashBitmap(splashAASize * bitmap->width, splashAASize,
			     1, splashModeMono1, gFalse);
    for (i = 0; i <= splashAASize * splashAASize; ++i) {
      aaGamma[i] = (Guchar)splashRound(
		       splashPow((SplashCoord)i /
				 (SplashCoord)(splashAASize * splashAASize),
				 splashAAGamma) * 255);
    }
  } else {
    aaBuf = NULL;
  }
  minLineWidth = 0;
  clearModRegion();
  debugMode = gFalse;
}
