SplashError Splash::composite(SplashBitmap *src, int xSrc, int ySrc,
			      int xDest, int yDest, int w, int h,
			      GBool noClip, GBool nonIsolated,
			      GBool knockout, SplashCoord knockoutOpacity) {
  SplashPipe pipe;
  SplashColor pixel;
  Guchar alpha;
  Guchar *ap;
  int x, y;

  if (src->mode != bitmap->mode) {
    return splashErrModeMismatch;
  }

  if(src->getSeparationList()->getLength() > bitmap->getSeparationList()->getLength()) {
    for (x = bitmap->getSeparationList()->getLength(); x < src->getSeparationList()->getLength(); x++)
      bitmap->getSeparationList()->append(((GfxSeparationColorSpace *)src->getSeparationList()->get(x))->copy());
  }
  if (src->alpha) {
    pipeInit(&pipe, xDest, yDest, NULL, pixel,
	     (Guchar)splashRound(state->fillAlpha * 255), gTrue, nonIsolated,
	     knockout, (Guchar)splashRound(knockoutOpacity * 255));
    if (noClip) {
      for (y = 0; y < h; ++y) {
	pipeSetXY(&pipe, xDest, yDest + y);
	ap = src->getAlphaPtr() + (ySrc + y) * src->getWidth() + xSrc;
	for (x = 0; x < w; ++x) {
	  src->getPixel(xSrc + x, ySrc + y, pixel);
	  alpha = *ap++;
    pipe.shape = alpha;
	  (this->*pipe.run)(&pipe);
	}
      }
      updateModX(xDest);
      updateModX(xDest + w - 1);
      updateModY(yDest);
      updateModY(yDest + h - 1);
    } else {
      for (y = 0; y < h; ++y) {
	pipeSetXY(&pipe, xDest, yDest + y);
	ap = src->getAlphaPtr() + (ySrc + y) * src->getWidth() + xSrc;
	for (x = 0; x < w; ++x) {
	  src->getPixel(xSrc + x, ySrc + y, pixel);
	  alpha = *ap++;
	  if (state->clip->test(xDest + x, yDest + y)) {
      pipe.shape = alpha;
	    (this->*pipe.run)(&pipe);
	    updateModX(xDest + x);
	    updateModY(yDest + y);
	  } else {
	    pipeIncX(&pipe);
	  }
	}
      }
    }
  } else {
    pipeInit(&pipe, xDest, yDest, NULL, pixel,
	     (Guchar)splashRound(state->fillAlpha * 255), gFalse, nonIsolated);
    if (noClip) {
      for (y = 0; y < h; ++y) {
	pipeSetXY(&pipe, xDest, yDest + y);
	for (x = 0; x < w; ++x) {
	  src->getPixel(xSrc + x, ySrc + y, pixel);
	  (this->*pipe.run)(&pipe);
	}
      }
      updateModX(xDest);
      updateModX(xDest + w - 1);
      updateModY(yDest);
      updateModY(yDest + h - 1);
    } else {
      for (y = 0; y < h; ++y) {
	pipeSetXY(&pipe, xDest, yDest + y);
	for (x = 0; x < w; ++x) {
	  src->getPixel(xSrc + x, ySrc + y, pixel);
	  if (state->clip->test(xDest + x, yDest + y)) {
	    (this->*pipe.run)(&pipe);
	    updateModX(xDest + x);
	    updateModY(yDest + y);
	  } else {
	    pipeIncX(&pipe);
	  }
	}
      }
    }
  }

  return splashOk;
}
