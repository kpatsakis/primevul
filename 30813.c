void Splash::scaleMaskYuXu(SplashImageMaskSource src, void *srcData,
			   int srcWidth, int srcHeight,
			   int scaledWidth, int scaledHeight,
			   SplashBitmap *dest) {
  Guchar *lineBuf;
  Guint pix;
  Guchar *destPtr0, *destPtr;
  int yp, yq, xp, xq, yt, y, yStep, xt, x, xStep, xx;
  int i, j;

  destPtr0 = dest->data;
  if (destPtr0 == NULL) {
    error(errInternal, -1, "dest->data is NULL in Splash::scaleMaskYuXu");
    return;
  }

  yp = scaledHeight / srcHeight;
  yq = scaledHeight % srcHeight;

  xp = scaledWidth / srcWidth;
  xq = scaledWidth % srcWidth;

  lineBuf = (Guchar *)gmalloc(srcWidth);

  yt = 0;

  for (y = 0; y < srcHeight; ++y) {

    if ((yt += yq) >= srcHeight) {
      yt -= srcHeight;
      yStep = yp + 1;
    } else {
      yStep = yp;
    }

    (*src)(srcData, lineBuf);

    xt = 0;

    xx = 0;
    for (x = 0; x < srcWidth; ++x) {

      if ((xt += xq) >= srcWidth) {
	xt -= srcWidth;
	xStep = xp + 1;
      } else {
	xStep = xp;
      }

      pix = lineBuf[x] ? 255 : 0;

      for (i = 0; i < yStep; ++i) {
	for (j = 0; j < xStep; ++j) {
	  destPtr = destPtr0 + i * scaledWidth + xx + j;
	  *destPtr++ = (Guchar)pix;
	}
      }

      xx += xStep;
    }

    destPtr0 += yStep * scaledWidth;
  }

  gfree(lineBuf);
}
