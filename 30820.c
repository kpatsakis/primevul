void Splash::strokeNarrow(SplashPath *path) {
  SplashPipe pipe;
  SplashXPath *xPath;
  SplashXPathSeg *seg;
  int x0, x1, y0, y1, xa, xb, y;
  SplashCoord dxdy;
  SplashClipResult clipRes;
  int nClipRes[3];
  int i;

  nClipRes[0] = nClipRes[1] = nClipRes[2] = 0;

  xPath = new SplashXPath(path, state->matrix, state->flatness, gFalse);

  pipeInit(&pipe, 0, 0, state->strokePattern, NULL,
	   (Guchar)splashRound(state->strokeAlpha * 255),
	   gFalse, gFalse);

  for (i = 0, seg = xPath->segs; i < xPath->length; ++i, ++seg) {
    if (seg->y0 <= seg->y1) {
      y0 = splashFloor(seg->y0);
      y1 = splashFloor(seg->y1);
      x0 = splashFloor(seg->x0);
      x1 = splashFloor(seg->x1);
    } else {
      y0 = splashFloor(seg->y1);
      y1 = splashFloor(seg->y0);
      x0 = splashFloor(seg->x1);
      x1 = splashFloor(seg->x0);
    }
    if ((clipRes = state->clip->testRect(x0 <= x1 ? x0 : x1, y0,
					 x0 <= x1 ? x1 : x0, y1))
	!= splashClipAllOutside) {
      if (y0 == y1) {
	if (x0 <= x1) {
	  drawSpan(&pipe, x0, x1, y0, clipRes == splashClipAllInside);
	} else {
	  drawSpan(&pipe, x1, x0, y0, clipRes == splashClipAllInside);
	}
      } else {
	dxdy = seg->dxdy;
	if (y0 < state->clip->getYMinI()) {
	  y0 = state->clip->getYMinI();
	  x0 = splashFloor(seg->x0 + ((SplashCoord)y0 - seg->y0) * dxdy);
	}
	if (y1 > state->clip->getYMaxI()) {
	  y1 = state->clip->getYMaxI();
	  x1 = splashFloor(seg->x0 + ((SplashCoord)y1 - seg->y0) * dxdy);
	}
	if (x0 <= x1) {
	  xa = x0;
	  for (y = y0; y <= y1; ++y) {
	    if (y < y1) {
	      xb = splashFloor(seg->x0 +
			       ((SplashCoord)y + 1 - seg->y0) * dxdy);
	    } else {
	      xb = x1 + 1;
	    }
	    if (xa == xb) {
	      drawPixel(&pipe, xa, y, clipRes == splashClipAllInside);
	    } else {
	      drawSpan(&pipe, xa, xb - 1, y, clipRes == splashClipAllInside);
	    }
	    xa = xb;
	  }
	} else {
	  xa = x0;
	  for (y = y0; y <= y1; ++y) {
	    if (y < y1) {
	      xb = splashFloor(seg->x0 +
			       ((SplashCoord)y + 1 - seg->y0) * dxdy);
	    } else {
	      xb = x1 - 1;
	    }
	    if (xa == xb) {
	      drawPixel(&pipe, xa, y, clipRes == splashClipAllInside);
	    } else {
	      drawSpan(&pipe, xb + 1, xa, y, clipRes == splashClipAllInside);
	    }
	    xa = xb;
	  }
	}
      }
    }
    ++nClipRes[clipRes];
  }
  if (nClipRes[splashClipPartial] ||
      (nClipRes[splashClipAllInside] && nClipRes[splashClipAllOutside])) {
    opClipRes = splashClipPartial;
  } else if (nClipRes[splashClipAllInside]) {
    opClipRes = splashClipAllInside;
  } else {
    opClipRes = splashClipAllOutside;
  }

  delete xPath;
}
