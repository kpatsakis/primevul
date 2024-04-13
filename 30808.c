void Splash::scaleImageYuXuBilinear(SplashImageSource src, void *srcData,
                                    SplashColorMode srcMode, int nComps,
                                    GBool srcAlpha, int srcWidth, int srcHeight,
                                    int scaledWidth, int scaledHeight,
                                    SplashBitmap *dest) {
  Guchar *srcBuf, *lineBuf1, *lineBuf2, *alphaSrcBuf, *alphaLineBuf1, *alphaLineBuf2;
  Guint pix[splashMaxColorComps];
  Guchar *destPtr0, *destPtr, *destAlphaPtr0, *destAlphaPtr;
  int i;

  srcBuf = (Guchar *)gmallocn(srcWidth+1, nComps); // + 1 pixel of padding
  lineBuf1 = (Guchar *)gmallocn(scaledWidth, nComps);
  lineBuf2 = (Guchar *)gmallocn(scaledWidth, nComps);
  if (srcAlpha) {
    alphaSrcBuf = (Guchar *)gmalloc(srcWidth+1); // + 1 pixel of padding
    alphaLineBuf1 = (Guchar *)gmalloc(scaledWidth);
    alphaLineBuf2 = (Guchar *)gmalloc(scaledWidth);
  } else {
    alphaSrcBuf = NULL;
    alphaLineBuf1 = NULL;
    alphaLineBuf2 = NULL;
  }

  double ySrc = 0.0;
  double yStep = (double)srcHeight/scaledHeight;
  double yFrac, yInt;
  int currentSrcRow = -1;
  (*src)(srcData, srcBuf, alphaSrcBuf);
  expandRow(srcBuf, lineBuf2, srcWidth, scaledWidth, nComps);
  if (srcAlpha)
    expandRow(alphaSrcBuf, alphaLineBuf2, srcWidth, scaledWidth, 1);

  destPtr0 = dest->data;
  destAlphaPtr0 = dest->alpha;
  for (int y = 0; y < scaledHeight; y++) {
    yFrac = modf(ySrc, &yInt);
    if ((int)yInt > currentSrcRow) {
      currentSrcRow++;
      memcpy(lineBuf1, lineBuf2, scaledWidth * nComps);
      if (srcAlpha)
        memcpy(alphaLineBuf1, alphaLineBuf2, scaledWidth);
      if (currentSrcRow < srcHeight) {
        (*src)(srcData, srcBuf, alphaSrcBuf);
        expandRow(srcBuf, lineBuf2, srcWidth, scaledWidth, nComps);
        if (srcAlpha)
          expandRow(alphaSrcBuf, alphaLineBuf2, srcWidth, scaledWidth, 1);
      }
    }

    for (int x = 0; x < scaledWidth; ++x) {
      for (i = 0; i < nComps; ++i) {
	pix[i] = lineBuf1[x*nComps + i]*(1.0 - yFrac) + lineBuf2[x*nComps + i]*yFrac;
      }

      destPtr = destPtr0 + (y * scaledWidth + x) * nComps;
      switch (srcMode) {
        case splashModeMono1: // mono1 is not allowed
          break;
        case splashModeMono8:
          *destPtr++ = (Guchar)pix[0];
          break;
        case splashModeRGB8:
          *destPtr++ = (Guchar)pix[0];
          *destPtr++ = (Guchar)pix[1];
          *destPtr++ = (Guchar)pix[2];
          break;
        case splashModeXBGR8:
          *destPtr++ = (Guchar)pix[2];
          *destPtr++ = (Guchar)pix[1];
          *destPtr++ = (Guchar)pix[0];
          *destPtr++ = (Guchar)255;
          break;
        case splashModeBGR8:
          *destPtr++ = (Guchar)pix[2];
          *destPtr++ = (Guchar)pix[1];
          *destPtr++ = (Guchar)pix[0];
          break;
#if SPLASH_CMYK
        case splashModeCMYK8:
          *destPtr++ = (Guchar)pix[0];
          *destPtr++ = (Guchar)pix[1];
          *destPtr++ = (Guchar)pix[2];
          *destPtr++ = (Guchar)pix[3];
          break;
        case splashModeDeviceN8:
          for (int cp = 0; cp < SPOT_NCOMPS+4; cp++)
            *destPtr++ = (Guchar)pix[cp];
          break;
#endif
      }

      if (srcAlpha) {
        destAlphaPtr = destAlphaPtr0 + y*scaledWidth + x;
        *destAlphaPtr = alphaLineBuf1[x]*(1.0 - yFrac) + alphaLineBuf2[x]*yFrac;
      }
    }

    ySrc += yStep;
  }

  gfree(alphaSrcBuf);
  gfree(alphaLineBuf1);
  gfree(alphaLineBuf2);
  gfree(srcBuf);
  gfree(lineBuf1);
  gfree(lineBuf2);
}
