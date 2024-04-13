void Splash::scaleImageYuXd(SplashImageSource src, void *srcData,
			    SplashColorMode srcMode, int nComps,
			    GBool srcAlpha, int srcWidth, int srcHeight,
			    int scaledWidth, int scaledHeight,
			    SplashBitmap *dest) {
  Guchar *lineBuf, *alphaLineBuf;
  Guint pix[splashMaxColorComps];
  Guint alpha;
  Guchar *destPtr0, *destPtr, *destAlphaPtr0, *destAlphaPtr;
  int yp, yq, xp, xq, yt, y, yStep, xt, x, xStep, xx, xxa, d, d0, d1;
  int i, j;

  yp = scaledHeight / srcHeight;
  yq = scaledHeight % srcHeight;

  xp = srcWidth / scaledWidth;
  xq = srcWidth % scaledWidth;

  lineBuf = (Guchar *)gmallocn(srcWidth, nComps);
  if (srcAlpha) {
    alphaLineBuf = (Guchar *)gmalloc(srcWidth);
  } else {
    alphaLineBuf = NULL;
  }

  yt = 0;

  destPtr0 = dest->data;
  destAlphaPtr0 = dest->alpha;
  for (y = 0; y < srcHeight; ++y) {

    if ((yt += yq) >= srcHeight) {
      yt -= srcHeight;
      yStep = yp + 1;
    } else {
      yStep = yp;
    }

    (*src)(srcData, lineBuf, alphaLineBuf);

    xt = 0;
    d0 = (1 << 23) / xp;
    d1 = (1 << 23) / (xp + 1);

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

      for (i = 0; i < nComps; ++i) {
	pix[i] = 0;
      }
      for (i = 0; i < xStep; ++i) {
	for (j = 0; j < nComps; ++j, ++xx) {
	  pix[j] += lineBuf[xx];
	}
      }
      for (i = 0; i < nComps; ++i) {
	pix[i] = (pix[i] * d) >> 23;
      }

      switch (srcMode) {
      case splashModeMono1: // mono1 is not allowed
	break;
      case splashModeMono8:
	for (i = 0; i < yStep; ++i) {
	  destPtr = destPtr0 + (i * scaledWidth + x) * nComps;
	  *destPtr++ = (Guchar)pix[0];
	}
	break;
      case splashModeRGB8:
	for (i = 0; i < yStep; ++i) {
	  destPtr = destPtr0 + (i * scaledWidth + x) * nComps;
	  *destPtr++ = (Guchar)pix[0];
	  *destPtr++ = (Guchar)pix[1];
	  *destPtr++ = (Guchar)pix[2];
	}
	break;
      case splashModeXBGR8:
	for (i = 0; i < yStep; ++i) {
	  destPtr = destPtr0 + (i * scaledWidth + x) * nComps;
	  *destPtr++ = (Guchar)pix[2];
	  *destPtr++ = (Guchar)pix[1];
	  *destPtr++ = (Guchar)pix[0];
	  *destPtr++ = (Guchar)255;
	}
	break;
      case splashModeBGR8:
	for (i = 0; i < yStep; ++i) {
	  destPtr = destPtr0 + (i * scaledWidth + x) * nComps;
	  *destPtr++ = (Guchar)pix[2];
	  *destPtr++ = (Guchar)pix[1];
	  *destPtr++ = (Guchar)pix[0];
	}
	break;
#if SPLASH_CMYK
      case splashModeCMYK8:
	for (i = 0; i < yStep; ++i) {
	  destPtr = destPtr0 + (i * scaledWidth + x) * nComps;
	  *destPtr++ = (Guchar)pix[0];
	  *destPtr++ = (Guchar)pix[1];
	  *destPtr++ = (Guchar)pix[2];
	  *destPtr++ = (Guchar)pix[3];
	}
	break;
      case splashModeDeviceN8:
	for (i = 0; i < yStep; ++i) {
	  destPtr = destPtr0 + (i * scaledWidth + x) * nComps;
    for (int cp = 0; cp < SPOT_NCOMPS+4; cp++)
      *destPtr++ = (Guchar)pix[cp];
	}
	break;
#endif
      }

      if (srcAlpha) {
	alpha = 0;
	for (i = 0; i < xStep; ++i, ++xxa) {
	  alpha += alphaLineBuf[xxa];
	}
	alpha = (alpha * d) >> 23;
	for (i = 0; i < yStep; ++i) {
	  destAlphaPtr = destAlphaPtr0 + i * scaledWidth + x;
	  *destAlphaPtr = (Guchar)alpha;
	}
      }
    }

    destPtr0 += yStep * scaledWidth * nComps;
    if (srcAlpha) {
      destAlphaPtr0 += yStep * scaledWidth;
    }
  }

  gfree(alphaLineBuf);
  gfree(lineBuf);
}
