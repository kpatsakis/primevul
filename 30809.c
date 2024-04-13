SplashBitmap *Splash::scaleMask(SplashImageMaskSource src, void *srcData,
				int srcWidth, int srcHeight,
				int scaledWidth, int scaledHeight) {
  SplashBitmap *dest;

  dest = new SplashBitmap(scaledWidth, scaledHeight, 1, splashModeMono8,
			  gFalse);
  if (scaledHeight < srcHeight) {
    if (scaledWidth < srcWidth) {
      scaleMaskYdXd(src, srcData, srcWidth, srcHeight,
		    scaledWidth, scaledHeight, dest);
    } else {
      scaleMaskYdXu(src, srcData, srcWidth, srcHeight,
		    scaledWidth, scaledHeight, dest);
    }
  } else {
    if (scaledWidth < srcWidth) {
      scaleMaskYuXd(src, srcData, srcWidth, srcHeight,
		    scaledWidth, scaledHeight, dest);
    } else {
      scaleMaskYuXu(src, srcData, srcWidth, srcHeight,
		    scaledWidth, scaledHeight, dest);
    }
  }
  return dest;
}
