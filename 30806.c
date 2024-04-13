void Splash::scaleImageYdXu(SplashImageSource src, void *srcData,
			    SplashColorMode srcMode, int nComps,
			    GBool srcAlpha, int srcWidth, int srcHeight,
			    int scaledWidth, int scaledHeight,
			    SplashBitmap *dest) {
  Guchar *lineBuf, *alphaLineBuf;
  Guint *pixBuf, *alphaPixBuf;
  Guint pix[splashMaxColorComps];
  Guint alpha;
  Guchar *destPtr, *destAlphaPtr;
  int yp, yq, xp, xq, yt, y, yStep, xt, x, xStep, d;
  int i, j;

  yp = srcHeight / scaledHeight;
  yq = srcHeight % scaledHeight;

  xp = scaledWidth / srcWidth;
  xq = scaledWidth % srcWidth;

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
    d = (1 << 23) / yStep;

    for (x = 0; x < srcWidth; ++x) {

      if ((xt += xq) >= srcWidth) {
	xt -= srcWidth;
	xStep = xp + 1;
      } else {
	xStep = xp;
      }

      for (i = 0; i < nComps; ++i) {
	pix[i] = (pixBuf[x * nComps + i] * d) >> 23;
      }

      switch (srcMode) {
      case splashModeMono1: // mono1 is not allowed
	break;
      case splashModeMono8:
	for (i = 0; i < xStep; ++i) {
	  *destPtr++ = (Guchar)pix[0];
	}
	break;
      case splashModeRGB8:
	for (i = 0; i < xStep; ++i) {
	  *destPtr++ = (Guchar)pix[0];
	  *destPtr++ = (Guchar)pix[1];
	  *destPtr++ = (Guchar)pix[2];
	}
	break;
      case splashModeXBGR8:
	for (i = 0; i < xStep; ++i) {
	  *destPtr++ = (Guchar)pix[2];
	  *destPtr++ = (Guchar)pix[1];
	  *destPtr++ = (Guchar)pix[0];
	  *destPtr++ = (Guchar)255;
	}
	break;
      case splashModeBGR8:
	for (i = 0; i < xStep; ++i) {
	  *destPtr++ = (Guchar)pix[2];
	  *destPtr++ = (Guchar)pix[1];
	  *destPtr++ = (Guchar)pix[0];
	}
	break;
#if SPLASH_CMYK
      case splashModeCMYK8:
	for (i = 0; i < xStep; ++i) {
	  *destPtr++ = (Guchar)pix[0];
	  *destPtr++ = (Guchar)pix[1];
	  *destPtr++ = (Guchar)pix[2];
	  *destPtr++ = (Guchar)pix[3];
	}
	break;
      case splashModeDeviceN8:
	for (i = 0; i < xStep; ++i) {
    for (int cp = 0; cp < SPOT_NCOMPS+4; cp++)
      *destPtr++ = (Guchar)pix[cp];
	}
	break;
#endif
      }

      if (srcAlpha) {
	alpha = (alphaPixBuf[x] * d) >> 23;
	for (i = 0; i < xStep; ++i) {
	  *destAlphaPtr++ = (Guchar)alpha;
	}
      }
    }
  }

  gfree(alphaPixBuf);
  gfree(alphaLineBuf);
  gfree(pixBuf);
  gfree(lineBuf);
}
