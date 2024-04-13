void Splash::pipeRunAAMono8(SplashPipe *pipe) {
  Guchar aSrc, aDest, alpha2, aResult;
  SplashColor cDest;
  Guchar cResult0;

  cDest[0] = *pipe->destColorPtr;
  aDest = *pipe->destAlphaPtr;

  aSrc = div255(pipe->aInput * pipe->shape);

  aResult = aSrc + aDest - div255(aSrc * aDest);
  alpha2 = aResult;

  if (alpha2 == 0) {
    cResult0 = 0;
  } else {
    cResult0 = state->grayTransfer[(Guchar)(((alpha2 - aSrc) * cDest[0] +
					     aSrc * pipe->cSrc[0]) / alpha2)];
  }

  *pipe->destColorPtr++ = cResult0;
  *pipe->destAlphaPtr++ = aResult;

  ++pipe->x;
}
