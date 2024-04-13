SplashError Splash::drawImage(SplashImageSource src, void *srcData,
			      SplashColorMode srcMode, GBool srcAlpha,
			      int w, int h, SplashCoord *mat, GBool interpolate,
			      GBool tilingPattern) {
  GBool ok;
  SplashBitmap *scaledImg;
  SplashClipResult clipRes;
  GBool minorAxisZero;
  int x0, y0, x1, y1, scaledWidth, scaledHeight;
  int nComps;
  int yp;

  if (debugMode) {
    printf("drawImage: srcMode=%d srcAlpha=%d w=%d h=%d mat=[%.2f %.2f %.2f %.2f %.2f %.2f]\n",
	   srcMode, srcAlpha, w, h, (double)mat[0], (double)mat[1], (double)mat[2],
	   (double)mat[3], (double)mat[4], (double)mat[5]);
  }

  ok = gFalse; // make gcc happy
  nComps = 0; // make gcc happy
  switch (bitmap->mode) {
  case splashModeMono1:
  case splashModeMono8:
    ok = srcMode == splashModeMono8;
    nComps = 1;
    break;
  case splashModeRGB8:
    ok = srcMode == splashModeRGB8;
    nComps = 3;
    break;
  case splashModeXBGR8:
    ok = srcMode == splashModeXBGR8;
    nComps = 4;
    break;
  case splashModeBGR8:
    ok = srcMode == splashModeBGR8;
    nComps = 3;
    break;
#if SPLASH_CMYK
  case splashModeCMYK8:
    ok = srcMode == splashModeCMYK8;
    nComps = 4;
    break;
  case splashModeDeviceN8:
    ok = srcMode == splashModeDeviceN8;
    nComps = SPOT_NCOMPS+4;
    break;
#endif
  default:
    ok = gFalse;
    break;
  }
  if (!ok) {
    return splashErrModeMismatch;
  }

  if (!splashCheckDet(mat[0], mat[1], mat[2], mat[3], 0.000001)) {
    return splashErrSingularMatrix;
  }

  minorAxisZero = mat[1] == 0 && mat[2] == 0;

  if (mat[0] > 0 && minorAxisZero && mat[3] > 0) {
    x0 = imgCoordMungeLower(mat[4]);
    y0 = imgCoordMungeLower(mat[5]);
    x1 = imgCoordMungeUpper(mat[0] + mat[4]);
    y1 = imgCoordMungeUpper(mat[3] + mat[5]);
    if (x0 == x1) {
      ++x1;
    }
    if (y0 == y1) {
      ++y1;
    }
    clipRes = state->clip->testRect(x0, y0, x1 - 1, y1 - 1);
    opClipRes = clipRes;
    if (clipRes != splashClipAllOutside) {
      scaledWidth = x1 - x0;
      scaledHeight = y1 - y0;
      yp = h / scaledHeight;
      if (yp < 0 || yp > INT_MAX - 1) {
        return splashErrBadArg;
      }
      scaledImg = scaleImage(src, srcData, srcMode, nComps, srcAlpha, w, h,
			     scaledWidth, scaledHeight, interpolate);
      if (scaledImg == NULL) {
        return splashErrBadArg;
      }
      blitImage(scaledImg, srcAlpha, x0, y0, clipRes);
      delete scaledImg;
    }

  } else if (mat[0] > 0 && minorAxisZero && mat[3] < 0) {
    x0 = imgCoordMungeLower(mat[4]);
    y0 = imgCoordMungeLower(mat[3] + mat[5]);
    x1 = imgCoordMungeUpper(mat[0] + mat[4]);
    y1 = imgCoordMungeUpper(mat[5]);
    if (x0 == x1) {
      if (mat[4] + mat[0] * 0.5 < x0) {
	--x0;
      } else {
	++x1;
      }
    }
    if (y0 == y1) {
      if (mat[5] + mat[1] * 0.5 < y0) {
	--y0;
      } else {
	++y1;
      }
    }
    clipRes = state->clip->testRect(x0, y0, x1 - 1, y1 - 1);
    opClipRes = clipRes;
    if (clipRes != splashClipAllOutside) {
      scaledWidth = x1 - x0;
      scaledHeight = y1 - y0;
      yp = h / scaledHeight;
      if (yp < 0 || yp > INT_MAX - 1) {
        return splashErrBadArg;
      }
      scaledImg = scaleImage(src, srcData, srcMode, nComps, srcAlpha, w, h,
			     scaledWidth, scaledHeight, interpolate);
      if (scaledImg == NULL) {
        return splashErrBadArg;
      }
      vertFlipImage(scaledImg, scaledWidth, scaledHeight, nComps);
      blitImage(scaledImg, srcAlpha, x0, y0, clipRes);
      delete scaledImg;
    }

  } else {
    return arbitraryTransformImage(src, srcData, srcMode, nComps, srcAlpha,
			    w, h, mat, interpolate, tilingPattern);
  }

  return splashOk;
}
