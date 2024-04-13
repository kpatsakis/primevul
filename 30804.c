SplashBitmap *Splash::scaleImage(SplashImageSource src, void *srcData,
				 SplashColorMode srcMode, int nComps,
				 GBool srcAlpha, int srcWidth, int srcHeight,
				 int scaledWidth, int scaledHeight, GBool interpolate) {
  SplashBitmap *dest;

  dest = new SplashBitmap(scaledWidth, scaledHeight, 1, srcMode, srcAlpha, gTrue, bitmap->getSeparationList());
  if (dest->getDataPtr() != NULL) {
    if (scaledHeight < srcHeight) {
      if (scaledWidth < srcWidth) {
	scaleImageYdXd(src, srcData, srcMode, nComps, srcAlpha,
		      srcWidth, srcHeight, scaledWidth, scaledHeight, dest);
      } else {
	scaleImageYdXu(src, srcData, srcMode, nComps, srcAlpha,
		      srcWidth, srcHeight, scaledWidth, scaledHeight, dest);
      }
    } else {
      if (scaledWidth < srcWidth) {
	scaleImageYuXd(src, srcData, srcMode, nComps, srcAlpha,
		      srcWidth, srcHeight, scaledWidth, scaledHeight, dest);
      } else {
	if (isImageInterpolationRequired(srcWidth, srcHeight, scaledWidth, scaledHeight, interpolate)) {
	  scaleImageYuXuBilinear(src, srcData, srcMode, nComps, srcAlpha,
				srcWidth, srcHeight, scaledWidth, scaledHeight, dest);
	} else {
	  scaleImageYuXu(src, srcData, srcMode, nComps, srcAlpha,
			srcWidth, srcHeight, scaledWidth, scaledHeight, dest);
	}
      }
    }
  } else {
    delete dest;
    dest = NULL;
  }
  return dest;
}
