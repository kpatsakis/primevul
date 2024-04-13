void Splash::scaleImageYdXd(SplashImageSource src, void *srcData,
			    SplashColorMode srcMode, int nComps,
			    GBool srcAlpha, int srcWidth, int srcHeight,
			    int scaledWidth, int scaledHeight,
			    SplashBitmap *dest) {
  Guchar *lineBuf, *alphaLineBuf;
  Guint *pixBuf, *alphaPixBuf;
  Guint pix0, pix1, pix2;
#if SPLASH_CMYK
  Guint pix3;
  Guint pix[SPOT_NCOMPS+4], cp;
#endif
  Guint alpha;
  Guchar *destPtr, *destAlphaPtr;
  int yp, yq, xp, xq, yt, y, yStep, xt, x, xStep, xx, xxa, d, d0, d1;
  int i, j;

  yp = srcHeight / scaledHeight;
  yq = srcHeight % scaledHeight;

  xp = srcWidth / scaledWidth;
  xq = srcWidth % scaledWidth;

  lineBuf = (Guchar *)gmallocn(srcWidth, nComps);
  pixBuf = (Guint *)gmallocn(srcWidth, nComps * sizeof(int));
  if (srcAlpha) {
    alphaLineBuf = (Guchar *)gmalloc(srcWidth);
    alphaPixBuf = (Guint *)gmallocn(srcWidth, sizeof(int));
  } else {
    alphaLineBuf = NULL;
    alphaPixBuf = NULL;
  }

  yt = 0;

  destPtr = dest->data;
  destAlphaPtr = dest->alpha;
  for (y = 0; y < scaledHeight; ++y) {

    if ((yt += yq) >= scaledHeight) {
      yt -= scaledHeight;
      yStep = yp + 1;
    } else {
      yStep = yp;
    }

    memset(pixBuf, 0, srcWidth * nComps * sizeof(int));
    if (srcAlpha) {
      memset(alphaPixBuf, 0, srcWidth * sizeof(int));
    }
    for (i = 0; i < yStep; ++i) {
      (*src)(srcData, lineBuf, alphaLineBuf);
      for (j = 0; j < srcWidth * nComps; ++j) {
	pixBuf[j] += lineBuf[j];
      }
      if (srcAlpha) {
	for (j = 0; j < srcWidth; ++j) {
	  alphaPixBuf[j] += alphaLineBuf[j];
	}
      }
    }

    xt = 0;
    d0 = (1 << 23) / (yStep * xp);
    d1 = (1 << 23) / (yStep * (xp + 1));

    xx = xxa = 0;
    for (x = 0; x < scaledWidth; ++x) {

      if ((xt += xq) >= scaledWidth) {
	xt -= scaledWidth;
	xStep = xp + 1;
	d = d1;
      } else {
	xStep = xp;
	d = d0;
      }

      switch (srcMode) {

      case splashModeMono8:

	pix0 = 0;
	for (i = 0; i < xStep; ++i) {
	  pix0 += pixBuf[xx++];
	}
	pix0 = (pix0 * d) >> 23;

	*destPtr++ = (Guchar)pix0;
	break;

      case splashModeRGB8:

	pix0 = pix1 = pix2 = 0;
	for (i = 0; i < xStep; ++i) {
	  pix0 += pixBuf[xx];
	  pix1 += pixBuf[xx+1];
	  pix2 += pixBuf[xx+2];
	  xx += 3;
	}
	pix0 = (pix0 * d) >> 23;
	pix1 = (pix1 * d) >> 23;
	pix2 = (pix2 * d) >> 23;

	*destPtr++ = (Guchar)pix0;
	*destPtr++ = (Guchar)pix1;
	*destPtr++ = (Guchar)pix2;
	break;

      case splashModeXBGR8:

	pix0 = pix1 = pix2 = 0;
	for (i = 0; i < xStep; ++i) {
	  pix0 += pixBuf[xx];
	  pix1 += pixBuf[xx+1];
	  pix2 += pixBuf[xx+2];
	  xx += 4;
	}
	pix0 = (pix0 * d) >> 23;
	pix1 = (pix1 * d) >> 23;
	pix2 = (pix2 * d) >> 23;

	*destPtr++ = (Guchar)pix2;
	*destPtr++ = (Guchar)pix1;
	*destPtr++ = (Guchar)pix0;
	*destPtr++ = (Guchar)255;
	break;

      case splashModeBGR8:

	pix0 = pix1 = pix2 = 0;
	for (i = 0; i < xStep; ++i) {
	  pix0 += pixBuf[xx];
	  pix1 += pixBuf[xx+1];
	  pix2 += pixBuf[xx+2];
	  xx += 3;
	}
	pix0 = (pix0 * d) >> 23;
	pix1 = (pix1 * d) >> 23;
	pix2 = (pix2 * d) >> 23;

	*destPtr++ = (Guchar)pix2;
	*destPtr++ = (Guchar)pix1;
	*destPtr++ = (Guchar)pix0;
	break;

#if SPLASH_CMYK
      case splashModeCMYK8:

	pix0 = pix1 = pix2 = pix3 = 0;
	for (i = 0; i < xStep; ++i) {
	  pix0 += pixBuf[xx];
	  pix1 += pixBuf[xx+1];
	  pix2 += pixBuf[xx+2];
	  pix3 += pixBuf[xx+3];
	  xx += 4;
	}
	pix0 = (pix0 * d) >> 23;
	pix1 = (pix1 * d) >> 23;
	pix2 = (pix2 * d) >> 23;
	pix3 = (pix3 * d) >> 23;

	*destPtr++ = (Guchar)pix0;
	*destPtr++ = (Guchar)pix1;
	*destPtr++ = (Guchar)pix2;
	*destPtr++ = (Guchar)pix3;
	break;
      case splashModeDeviceN8:

  for (cp = 0; cp < SPOT_NCOMPS+4; cp++)
    pix[cp] = 0;
	for (i = 0; i < xStep; ++i) {
    for (cp = 0; cp < SPOT_NCOMPS+4; cp++) {
      pix[cp] += pixBuf[xx + cp];
    }
    xx += (SPOT_NCOMPS+4);
	}
  for (cp = 0; cp < SPOT_NCOMPS+4; cp++)
    pix[cp] = (pix[cp] * d) >> 23;

  for (cp = 0; cp < SPOT_NCOMPS+4; cp++)
    *destPtr++ = (Guchar)pix[cp];
	break;
#endif


      case splashModeMono1: // mono1 is not allowed
      default:
	break;
      }

      if (srcAlpha) {
	alpha = 0;
	for (i = 0; i < xStep; ++i, ++xxa) {
	  alpha += alphaPixBuf[xxa];
	}
	alpha = (alpha * d) >> 23;
	*destAlphaPtr++ = (Guchar)alpha;
      }
    }
  }

  gfree(alphaPixBuf);
  gfree(alphaLineBuf);
  gfree(pixBuf);
  gfree(lineBuf);
}
