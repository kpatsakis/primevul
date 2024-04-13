void Splash::blitImageClipped(SplashBitmap *src, GBool srcAlpha,
			      int xSrc, int ySrc, int xDest, int yDest,
			      int w, int h) {
  SplashPipe pipe;
  SplashColor pixel;
  Guchar *ap;
  int x, y;

  if (vectorAntialias) {
    pipeInit(&pipe, xDest, yDest, NULL, pixel,
	     (Guchar)splashRound(state->fillAlpha * 255), gTrue, gFalse);
    drawAAPixelInit();
    if (srcAlpha) {
      for (y = 0; y < h; ++y) {
	ap = src->getAlphaPtr() + (ySrc + y) * src->getWidth() + xSrc;
	for (x = 0; x < w; ++x) {
	  src->getPixel(xSrc + x, ySrc + y, pixel);
	  pipe.shape = *ap++;
	  drawAAPixel(&pipe, xDest + x, yDest + y);
	}
      }
    } else {
      for (y = 0; y < h; ++y) {
	for (x = 0; x < w; ++x) {
	  src->getPixel(xSrc + x, ySrc + y, pixel);
	  pipe.shape = 255;
	  drawAAPixel(&pipe, xDest + x, yDest + y);
	}
      }
    }
  } else {
    pipeInit(&pipe, xDest, yDest, NULL, pixel,
	     (Guchar)splashRound(state->fillAlpha * 255), srcAlpha, gFalse);
    if (srcAlpha) {
      for (y = 0; y < h; ++y) {
	ap = src->getAlphaPtr() + (ySrc + y) * src->getWidth() + xSrc;
	pipeSetXY(&pipe, xDest, yDest + y);
	for (x = 0; x < w; ++x) {
	  if (state->clip->test(xDest + x, yDest + y)) {
	    src->getPixel(xSrc + x, ySrc + y, pixel);
	    pipe.shape = *ap++;
	    (this->*pipe.run)(&pipe);
	    updateModX(xDest + x);
	    updateModY(yDest + y);
	  } else {
	    pipeIncX(&pipe);
	    ++ap;
	  }
	}
      }
    } else {
      for (y = 0; y < h; ++y) {
	pipeSetXY(&pipe, xDest, yDest + y);
	for (x = 0; x < w; ++x) {
	  if (state->clip->test(xDest + x, yDest + y)) {
	    src->getPixel(xSrc + x, ySrc + y, pixel);
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
}
