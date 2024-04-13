SplashError Splash::blitTransparent(SplashBitmap *src, int xSrc, int ySrc,
				    int xDest, int yDest, int w, int h) {
  SplashColorPtr p, sp;
  Guchar *q;
  int x, y, mask, srcMask;

  if (src->mode != bitmap->mode) {
    return splashErrModeMismatch;
  }

  switch (bitmap->mode) {
  case splashModeMono1:
    for (y = 0; y < h; ++y) {
      p = &bitmap->data[(yDest + y) * bitmap->rowSize + (xDest >> 3)];
      mask = 0x80 >> (xDest & 7);
      sp = &src->data[(ySrc + y) * src->rowSize + (xSrc >> 3)];
      srcMask = 0x80 >> (xSrc & 7);
      for (x = 0; x < w; ++x) {
	if (*sp & srcMask) {
	  *p |= mask;
	} else {
	  *p &= ~mask;
	}
	if (!(mask >>= 1)) {
	  mask = 0x80;
	  ++p;
	}
	if (!(srcMask >>= 1)) {
	  srcMask = 0x80;
	  ++sp;
	}
      }
    }
    break;
  case splashModeMono8:
    for (y = 0; y < h; ++y) {
      p = &bitmap->data[(yDest + y) * bitmap->rowSize + xDest];
      sp = &src->data[(ySrc + y) * bitmap->rowSize + xSrc];
      for (x = 0; x < w; ++x) {
	*p++ = *sp++;
      }
    }
    break;
  case splashModeRGB8:
  case splashModeBGR8:
    for (y = 0; y < h; ++y) {
      p = &bitmap->data[(yDest + y) * bitmap->rowSize + 3 * xDest];
      sp = &src->data[(ySrc + y) * src->rowSize + 3 * xSrc];
      for (x = 0; x < w; ++x) {
	*p++ = *sp++;
	*p++ = *sp++;
	*p++ = *sp++;
      }
    }
    break;
  case splashModeXBGR8:
    for (y = 0; y < h; ++y) {
      p = &bitmap->data[(yDest + y) * bitmap->rowSize + 4 * xDest];
      sp = &src->data[(ySrc + y) * src->rowSize + 4 * xSrc];
      for (x = 0; x < w; ++x) {
	*p++ = *sp++;
	*p++ = *sp++;
	*p++ = *sp++;
	*p++ = 255;
	sp++;
      }
    }
    break;
#if SPLASH_CMYK
  case splashModeCMYK8:
    for (y = 0; y < h; ++y) {
      p = &bitmap->data[(yDest + y) * bitmap->rowSize + 4 * xDest];
      sp = &src->data[(ySrc + y) * src->rowSize + 4 * xSrc];
      for (x = 0; x < w; ++x) {
	*p++ = *sp++;
	*p++ = *sp++;
	*p++ = *sp++;
	*p++ = *sp++;
      }
    }
    break;
  case splashModeDeviceN8:
    for (y = 0; y < h; ++y) {
      p = &bitmap->data[(yDest + y) * bitmap->rowSize + (SPOT_NCOMPS+4) * xDest];
      sp = &src->data[(ySrc + y) * src->rowSize + (SPOT_NCOMPS+4) * xSrc];
      for (x = 0; x < w; ++x) {
        for (int cp=0; cp < SPOT_NCOMPS+4; cp++)
          *p++ = *sp++;
      }
    }
    break;
#endif
  }

  if (bitmap->alpha) {
    for (y = 0; y < h; ++y) {
      q = &bitmap->alpha[(yDest + y) * bitmap->width + xDest];
      memset(q, 0x00, w);
    }
  }

  return splashOk;
}
