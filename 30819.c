SplashError Splash::stroke(SplashPath *path) {
  SplashPath *path2, *dPath;
  SplashCoord d1, d2, t1, t2, w;

  if (debugMode) {
    printf("stroke [dash:%d] [width:%.2f]:\n",
	   state->lineDashLength, (double)state->lineWidth);
    dumpPath(path);
  }
  opClipRes = splashClipAllOutside;
  if (path->length == 0) {
    return splashErrEmptyPath;
  }
  path2 = flattenPath(path, state->matrix, state->flatness);
  if (state->lineDashLength > 0) {
    dPath = makeDashedPath(path2);
    delete path2;
    path2 = dPath;
    if (path2->length == 0) {
      delete path2;
      return splashErrEmptyPath;
    }
  }

  t1 = state->matrix[0] + state->matrix[2];
  t2 = state->matrix[1] + state->matrix[3];
  d1 = t1 * t1 + t2 * t2;
  t1 = state->matrix[0] - state->matrix[2];
  t2 = state->matrix[1] - state->matrix[3];
  d2 = t1 * t1 + t2 * t2;
  if (d2 > d1) {
    d1 = d2;
  }
  d1 *= 0.5;
  if (d1 > 0 &&
      d1 * state->lineWidth * state->lineWidth < minLineWidth * minLineWidth) {
    w = minLineWidth / splashSqrt(d1);
    strokeWide(path2, w);
  } else if (bitmap->mode == splashModeMono1) {
    if (d1 * state->lineWidth <= 2) {
      strokeNarrow(path2);
    } else {
      strokeWide(path2, state->lineWidth);
    }
  } else {
    if (state->lineWidth == 0) {
      strokeNarrow(path2);
    } else {
      strokeWide(path2, state->lineWidth);
    }
  }

  delete path2;
  return splashOk;
}
