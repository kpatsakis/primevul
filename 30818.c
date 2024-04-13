SplashError Splash::shadedFill(SplashPath *path, GBool hasBBox,
                               SplashPattern *pattern) {
  SplashPipe pipe;
  SplashXPath *xPath;
  SplashXPathScanner *scanner;
  int xMinI, yMinI, xMaxI, yMaxI, x0, x1, y;
  SplashClipResult clipRes;

  if (vectorAntialias && aaBuf == NULL) { // should not happen, but to be secure
    return splashErrGeneric;
  }
  if (path->length == 0) {
    return splashErrEmptyPath;
  }
  xPath = new SplashXPath(path, state->matrix, state->flatness, gTrue);
  if (vectorAntialias) {
    xPath->aaScale();
  }
  xPath->sort();
  yMinI = state->clip->getYMinI();
  yMaxI = state->clip->getYMaxI();
  if (vectorAntialias && !inShading) {
    yMinI = yMinI * splashAASize;
    yMaxI = (yMaxI + 1) * splashAASize - 1;
  }
  scanner = new SplashXPathScanner(xPath, gFalse, yMinI, yMaxI);

  if (vectorAntialias) {
    scanner->getBBoxAA(&xMinI, &yMinI, &xMaxI, &yMaxI);
  } else {
    scanner->getBBox(&xMinI, &yMinI, &xMaxI, &yMaxI);
  }

  if ((clipRes = state->clip->testRect(xMinI, yMinI, xMaxI, yMaxI)) != splashClipAllOutside) {
    if (yMinI < state->clip->getYMinI()) {
      yMinI = state->clip->getYMinI();
    }
    if (yMaxI > state->clip->getYMaxI()) {
      yMaxI = state->clip->getYMaxI();
    }

    pipeInit(&pipe, 0, yMinI, pattern, NULL, (Guchar)splashRound(state->fillAlpha * 255), vectorAntialias && !hasBBox, gFalse);

    if (vectorAntialias) {
      for (y = yMinI; y <= yMaxI; ++y) {
        scanner->renderAALine(aaBuf, &x0, &x1, y);
        if (clipRes != splashClipAllInside) {
          state->clip->clipAALine(aaBuf, &x0, &x1, y);
        }
#if splashAASize == 4
        if (!hasBBox && y > yMinI && y < yMaxI) {
          Guchar *p0, *p1, *p2, *p3;
          Guchar c1, c2, c3, c4;
          p0 = aaBuf->getDataPtr() + (x0 >> 1);
          p1 = p0 + aaBuf->getRowSize();
          p2 = p1 + aaBuf->getRowSize();
          p3 = p2 + aaBuf->getRowSize();
          if (x0 & 1) {
           c1 = (*p0 & 0x0f); c2 =(*p1 & 0x0f); c3 = (*p2 & 0x0f) ; c4 = (*p3 & 0x0f);
          } else {
            c1 = (*p0 >> 4); c2 = (*p1 >> 4); c3 = (*p2 >> 4); c4 = (*p3 >> 4);
          }
          if ( (c1 & 0x03) == 0x03 && (c2 & 0x03) == 0x03 && (c3 & 0x03) == 0x03 && (c4 & 0x03) == 0x03
            && c1 == c2 && c2 == c3 && c3 == c4 &&
            pattern->testPosition(x0 - 1, y) )
          {
            Guchar shapeCorrection = (x0 & 1) ? 0x0f : 0xf0;
            *p0 |= shapeCorrection;
            *p1 |= shapeCorrection;
            *p2 |= shapeCorrection;
            *p3 |= shapeCorrection;
          }
          p0 = aaBuf->getDataPtr() + (x1 >> 1);
          p1 = p0 + aaBuf->getRowSize();
          p2 = p1 + aaBuf->getRowSize();
          p3 = p2 + aaBuf->getRowSize();
          if (x1 & 1) {
            c1 = (*p0 & 0x0f); c2 =(*p1 & 0x0f); c3 = (*p2 & 0x0f) ; c4 = (*p3 & 0x0f);
          } else {
            c1 = (*p0 >> 4); c2 = (*p1 >> 4); c3 = (*p2 >> 4); c4 = (*p3 >> 4);
          }

          if ( (c1 & 0xc) == 0x0c && (c2 & 0x0c) == 0x0c && (c3 & 0x0c) == 0x0c && (c4 & 0x0c) == 0x0c
            && c1 == c2 && c2 == c3 && c3 == c4 &&
            pattern->testPosition(x1 + 1, y) )
          {
            Guchar shapeCorrection = (x1 & 1) ? 0x0f : 0xf0;
            *p0 |= shapeCorrection;
            *p1 |= shapeCorrection;
            *p2 |= shapeCorrection;
            *p3 |= shapeCorrection;
          }
        }
#endif
        drawAALine(&pipe, x0, x1, y);
      }
    } else {
      SplashClipResult clipRes2;
      for (y = yMinI; y <= yMaxI; ++y) {
        while (scanner->getNextSpan(y, &x0, &x1)) {
          if (clipRes == splashClipAllInside) {
            drawSpan(&pipe, x0, x1, y, gTrue);
          } else {
            if (x0 < state->clip->getXMinI()) {
              x0 = state->clip->getXMinI();
            }
            if (x1 > state->clip->getXMaxI()) {
              x1 = state->clip->getXMaxI();
            }
            clipRes2 = state->clip->testSpan(x0, x1, y);
            drawSpan(&pipe, x0, x1, y, clipRes2 == splashClipAllInside);
          }
        }
      }
    }
  }
  opClipRes = clipRes;

  delete scanner;
  delete xPath;
  return splashOk;
}
