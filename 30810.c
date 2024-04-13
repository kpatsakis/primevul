void Splash::scaleMaskYdXd(SplashImageMaskSource src, void *srcData,
			   int srcWidth, int srcHeight,
			   int scaledWidth, int scaledHeight,
			   SplashBitmap *dest) {
  Guchar *lineBuf;
  Guint *pixBuf;
  Guint pix;
  Guchar *destPtr;
  int yp, yq, xp, xq, yt, y, yStep, xt, x, xStep, xx, d, d0, d1;
  int i, j;

  yp = srcHeight / scaledHeight;
  yq = srcHeight % scaledHeight;

  xp = srcWidth / scaledWidth;
  xq = srcWidth % scaledWidth;

  lineBuf = (Guchar *)gmalloc(srcWidth);
  pixBuf = (Guint *)gmallocn(srcWidth, sizeof(int));

  yt = 0;

  destPtr = dest->data;
  for (y = 0; y < scaledHeight; ++y) {

    if ((yt += yq) >= scaledHeight) {
      yt -= scaledHeight;
      yStep = yp + 1;
    } else {
      yStep = yp;
    }

    memset(pixBuf, 0, srcWidth * sizeof(int));
    for (i = 0; i < yStep; ++i) {
      (*src)(srcData, lineBuf);
      for (j = 0; j < srcWidth; ++j) {
	pixBuf[j] += lineBuf[j];
      }
    }

    xt = 0;
    d0 = (255 << 23) / (yStep * xp);
    d1 = (255 << 23) / (yStep * (xp + 1));

    xx = 0;
    for (x = 0; x < scaledWidth; ++x) {

      if ((xt += xq) >= scaledWidth) {
	xt -= scaledWidth;
	xStep = xp + 1;
	d = d1;
      } else {
	xStep = xp;
	d = d0;
      }

      pix = 0;
      for (i = 0; i < xStep; ++i) {
	pix += pixBuf[xx++];
      }
      pix = (pix * d) >> 23;

      *destPtr++ = (Guchar)pix;
    }
  }

  gfree(pixBuf);
  gfree(lineBuf);
}
