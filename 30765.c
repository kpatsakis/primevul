SplashError Splash::arbitraryTransformImage(SplashImageSource src, void *srcData,
				     SplashColorMode srcMode, int nComps,
				     GBool srcAlpha,
				     int srcWidth, int srcHeight,
				     SplashCoord *mat, GBool interpolate,
             GBool tilingPattern) {
  SplashBitmap *scaledImg;
  SplashClipResult clipRes, clipRes2;
  SplashPipe pipe;
  SplashColor pixel;
  int scaledWidth, scaledHeight, t0, t1, th;
  SplashCoord r00, r01, r10, r11, det, ir00, ir01, ir10, ir11;
  SplashCoord vx[4], vy[4];
  int xMin, yMin, xMax, yMax;
  ImageSection section[3];
  int nSections;
  int y, xa, xb, x, i, xx, yy, yp;

  vx[0] = mat[4];                    vy[0] = mat[5];
  vx[1] = mat[2] + mat[4];           vy[1] = mat[3] + mat[5];
  vx[2] = mat[0] + mat[2] + mat[4];  vy[2] = mat[1] + mat[3] + mat[5];
  vx[3] = mat[0] + mat[4];           vy[3] = mat[1] + mat[5];

  xMin = imgCoordMungeLower(vx[0]);
  xMax = imgCoordMungeUpper(vx[0]);
  yMin = imgCoordMungeLower(vy[0]);
  yMax = imgCoordMungeUpper(vy[0]);
  for (i = 1; i < 4; ++i) {
    t0 = imgCoordMungeLower(vx[i]);
    if (t0 < xMin) {
      xMin = t0;
    }
    t0 = imgCoordMungeUpper(vx[i]);
    if (t0 > xMax) {
      xMax = t0;
    }
    t1 = imgCoordMungeLower(vy[i]);
    if (t1 < yMin) {
      yMin = t1;
    }
    t1 = imgCoordMungeUpper(vy[i]);
    if (t1 > yMax) {
      yMax = t1;
    }
  }
  clipRes = state->clip->testRect(xMin, yMin, xMax - 1, yMax - 1);
  opClipRes = clipRes;
  if (clipRes == splashClipAllOutside) {
    return splashOk;
  }

  if (splashAbs(mat[0]) >= splashAbs(mat[1])) {
    scaledWidth = xMax - xMin;
    scaledHeight = yMax - yMin;
  } else {
    scaledWidth = yMax - yMin;
    scaledHeight = xMax - xMin;
  }
  if (scaledHeight <= 1 || scaledHeight <= 1 || tilingPattern) {
    if (mat[0] >= 0) {
      t0 = imgCoordMungeUpper(mat[0] + mat[4]) - imgCoordMungeLower(mat[4]);
    } else {
      t0 = imgCoordMungeUpper(mat[4]) - imgCoordMungeLower(mat[0] + mat[4]);
    }
    if (mat[1] >= 0) {
      t1 = imgCoordMungeUpper(mat[1] + mat[5]) - imgCoordMungeLower(mat[5]);
    } else {
      t1 = imgCoordMungeUpper(mat[5]) - imgCoordMungeLower(mat[1] + mat[5]);
    }
    scaledWidth = t0 > t1 ? t0 : t1;
    if (mat[2] >= 0) {
      t0 = imgCoordMungeUpper(mat[2] + mat[4]) - imgCoordMungeLower(mat[4]);
      if (splashAbs(mat[1]) >= 1) {
        th = imgCoordMungeUpper(mat[2]) - imgCoordMungeLower(mat[0] * mat[3] / mat[1]);
	    if (th > t0) t0 = th;
      }
    } else {
      t0 = imgCoordMungeUpper(mat[4]) - imgCoordMungeLower(mat[2] + mat[4]);
      if (splashAbs(mat[1]) >= 1) {
        th = imgCoordMungeUpper(mat[0] * mat[3] / mat[1]) - imgCoordMungeLower(mat[2]);
        if (th > t0) t0 = th;
      }
    }
    if (mat[3] >= 0) {
      t1 = imgCoordMungeUpper(mat[3] + mat[5]) - imgCoordMungeLower(mat[5]);
      if (splashAbs(mat[0]) >= 1) {
        th = imgCoordMungeUpper(mat[3]) - imgCoordMungeLower(mat[1] * mat[2] / mat[0]);
	    if (th > t1) t1 = th;
      }
    } else {
      t1 = imgCoordMungeUpper(mat[5]) - imgCoordMungeLower(mat[3] + mat[5]);
      if (splashAbs(mat[0]) >= 1) {
        th = imgCoordMungeUpper(mat[1] * mat[2] / mat[0]) - imgCoordMungeLower(mat[3]);
	    if (th > t1) t1 = th;
      }
    }
    scaledHeight = t0 > t1 ? t0 : t1;
  }
  if (scaledWidth == 0) {
    scaledWidth = 1;
  }
  if (scaledHeight == 0) {
    scaledHeight = 1;
  }

  r00 = mat[0] / scaledWidth;
  r01 = mat[1] / scaledWidth;
  r10 = mat[2] / scaledHeight;
  r11 = mat[3] / scaledHeight;
  det = r00 * r11 - r01 * r10;
  if (splashAbs(det) < 1e-6) {
    return splashErrBadArg;
  }
  ir00 = r11 / det;
  ir01 = -r01 / det;
  ir10 = -r10 / det;
  ir11 = r00 / det;

  yp = srcHeight / scaledHeight;
  if (yp < 0 || yp > INT_MAX - 1) {
    return splashErrBadArg;
  }
  scaledImg = scaleImage(src, srcData, srcMode, nComps, srcAlpha,
			 srcWidth, srcHeight, scaledWidth, scaledHeight, interpolate);

  if (scaledImg == NULL) {
    return splashErrBadArg;
  }

  i = 0;
  if (vy[1] < vy[i]) {
    i = 1;
  }
  if (vy[2] < vy[i]) {
    i = 2;
  }
  if (vy[3] < vy[i]) {
    i = 3;
  }
  if (splashAbs(vy[i] - vy[(i-1) & 3]) <= 0.000001 &&
      vy[(i-1) & 3] < vy[(i+1) & 3]) {
    i = (i-1) & 3;
  }
  if (splashAbs(vy[i] - vy[(i+1) & 3]) <= 0.000001) {
    section[0].y0 = imgCoordMungeLower(vy[i]);
    section[0].y1 = imgCoordMungeUpper(vy[(i+2) & 3]) - 1;
    if (vx[i] < vx[(i+1) & 3]) {
      section[0].ia0 = i;
      section[0].ia1 = (i+3) & 3;
      section[0].ib0 = (i+1) & 3;
      section[0].ib1 = (i+2) & 3;
    } else {
      section[0].ia0 = (i+1) & 3;
      section[0].ia1 = (i+2) & 3;
      section[0].ib0 = i;
      section[0].ib1 = (i+3) & 3;
    }
    nSections = 1;
  } else {
    section[0].y0 = imgCoordMungeLower(vy[i]);
    section[2].y1 = imgCoordMungeUpper(vy[(i+2) & 3]) - 1;
    section[0].ia0 = section[0].ib0 = i;
    section[2].ia1 = section[2].ib1 = (i+2) & 3;
    if (vx[(i+1) & 3] < vx[(i+3) & 3]) {
      section[0].ia1 = section[2].ia0 = (i+1) & 3;
      section[0].ib1 = section[2].ib0 = (i+3) & 3;
    } else {
      section[0].ia1 = section[2].ia0 = (i+3) & 3;
      section[0].ib1 = section[2].ib0 = (i+1) & 3;
    }
    if (vy[(i+1) & 3] < vy[(i+3) & 3]) {
      section[1].y0 = imgCoordMungeLower(vy[(i+1) & 3]);
      section[2].y0 = imgCoordMungeUpper(vy[(i+3) & 3]);
      if (vx[(i+1) & 3] < vx[(i+3) & 3]) {
	section[1].ia0 = (i+1) & 3;
	section[1].ia1 = (i+2) & 3;
	section[1].ib0 = i;
	section[1].ib1 = (i+3) & 3;
      } else {
	section[1].ia0 = i;
	section[1].ia1 = (i+3) & 3;
	section[1].ib0 = (i+1) & 3;
	section[1].ib1 = (i+2) & 3;
      }
    } else {
      section[1].y0 = imgCoordMungeLower(vy[(i+3) & 3]);
      section[2].y0 = imgCoordMungeUpper(vy[(i+1) & 3]);
      if (vx[(i+1) & 3] < vx[(i+3) & 3]) {
	section[1].ia0 = i;
	section[1].ia1 = (i+1) & 3;
	section[1].ib0 = (i+3) & 3;
	section[1].ib1 = (i+2) & 3;
      } else {
	section[1].ia0 = (i+3) & 3;
	section[1].ia1 = (i+2) & 3;
	section[1].ib0 = i;
	section[1].ib1 = (i+1) & 3;
      }
    }
    section[0].y1 = section[1].y0 - 1;
    section[1].y1 = section[2].y0 - 1;
    nSections = 3;
  }
  for (i = 0; i < nSections; ++i) {
    section[i].xa0 = vx[section[i].ia0];
    section[i].ya0 = vy[section[i].ia0];
    section[i].xa1 = vx[section[i].ia1];
    section[i].ya1 = vy[section[i].ia1];
    section[i].xb0 = vx[section[i].ib0];
    section[i].yb0 = vy[section[i].ib0];
    section[i].xb1 = vx[section[i].ib1];
    section[i].yb1 = vy[section[i].ib1];
    section[i].dxdya = (section[i].xa1 - section[i].xa0) /
                       (section[i].ya1 - section[i].ya0);
    section[i].dxdyb = (section[i].xb1 - section[i].xb0) /
                       (section[i].yb1 - section[i].yb0);
  }

  pipeInit(&pipe, 0, 0, NULL, pixel,
	   (Guchar)splashRound(state->fillAlpha * 255),
	   srcAlpha || (vectorAntialias && clipRes != splashClipAllInside),
	   gFalse);
  if (vectorAntialias) {
    drawAAPixelInit();
  }

  if (nSections == 1) {
    if (section[0].y0 == section[0].y1) {
      ++section[0].y1;
      clipRes = opClipRes = splashClipPartial;
    }
  } else {
    if (section[0].y0 == section[2].y1) {
      ++section[1].y1;
      clipRes = opClipRes = splashClipPartial;
    }
  }

  for (i = 0; i < nSections; ++i) {
    for (y = section[i].y0; y <= section[i].y1; ++y) {
      xa = imgCoordMungeLower(section[i].xa0 +
			      ((SplashCoord)y + 0.5 - section[i].ya0) *
			        section[i].dxdya);
      xb = imgCoordMungeUpper(section[i].xb0 +
			      ((SplashCoord)y + 0.5 - section[i].yb0) *
			        section[i].dxdyb);
      if (xa == xb) {
	++xb;
      }
      if (clipRes != splashClipAllInside) {
	clipRes2 = state->clip->testSpan(xa, xb - 1, y);
      } else {
	clipRes2 = clipRes;
      }
      for (x = xa; x < xb; ++x) {
	xx = splashFloor(((SplashCoord)x + 0.5 - mat[4]) * ir00 +
			 ((SplashCoord)y + 0.5 - mat[5]) * ir10);
	yy = splashFloor(((SplashCoord)x + 0.5 - mat[4]) * ir01 +
			 ((SplashCoord)y + 0.5 - mat[5]) * ir11);
	if (xx < 0) {
	  xx = 0;
	} else if (xx >= scaledWidth) {
	  xx = scaledWidth - 1;
	}
	if (yy < 0) {
	  yy = 0;
	} else if (yy >= scaledHeight) {
	  yy = scaledHeight - 1;
	}
	scaledImg->getPixel(xx, yy, pixel);
	if (srcAlpha) {
	  pipe.shape = scaledImg->alpha[yy * scaledWidth + xx];
	} else {
	  pipe.shape = 255;
	}
	if (vectorAntialias && clipRes2 != splashClipAllInside) {
	  drawAAPixel(&pipe, x, y);
	} else {
	  drawPixel(&pipe, x, y, clipRes2 == splashClipAllInside);
	}
      }
    }
  }

  delete scaledImg;
  return splashOk;
}
