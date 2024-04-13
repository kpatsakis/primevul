void Splash::pipeRunAAXBGR8(SplashPipe *pipe) {
  Guchar aSrc, aDest, alpha2, aResult;
  SplashColor cDest;
  Guchar cResult0, cResult1, cResult2;

  cDest[0] = pipe->destColorPtr[2];
  cDest[1] = pipe->destColorPtr[1];
  cDest[2] = pipe->destColorPtr[0];
  aDest = *pipe->destAlphaPtr;

  aSrc = div255(pipe->aInput * pipe->shape);

  aResult = aSrc + aDest - div255(aSrc * aDest);
  alpha2 = aResult;

  if (alpha2 == 0) {
    cResult0 = 0;
    cResult1 = 0;
    cResult2 = 0;
  } else {
    cResult0 = state->rgbTransferR[(Guchar)(((alpha2 - aSrc) * cDest[0] +
					     aSrc * pipe->cSrc[0]) / alpha2)];
    cResult1 = state->rgbTransferG[(Guchar)(((alpha2 - aSrc) * cDest[1] +
					     aSrc * pipe->cSrc[1]) / alpha2)];
    cResult2 = state->rgbTransferB[(Guchar)(((alpha2 - aSrc) * cDest[2] +
					     aSrc * pipe->cSrc[2]) / alpha2)];
  }

  *pipe->destColorPtr++ = cResult2;
  *pipe->destColorPtr++ = cResult1;
  *pipe->destColorPtr++ = cResult0;
  *pipe->destColorPtr++ = 255;
  *pipe->destAlphaPtr++ = aResult;

  ++pipe->x;
}
