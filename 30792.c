void Splash::pipeRunAADeviceN8(SplashPipe *pipe) {
  Guchar aSrc, aDest, alpha2, aResult;
  SplashColor cDest;
  Guchar cResult[SPOT_NCOMPS+4];
  int cp, mask;

  for (cp=0; cp < SPOT_NCOMPS+4; cp++)
    cDest[cp] = pipe->destColorPtr[cp];
  aDest = *pipe->destAlphaPtr;

  aSrc = div255(pipe->aInput * pipe->shape);

  aResult = aSrc + aDest - div255(aSrc * aDest);
  alpha2 = aResult;

  if (alpha2 == 0) {
    for (cp=0; cp < SPOT_NCOMPS+4; cp++)
      cResult[cp] = 0;
  } else {
    for (cp=0; cp < SPOT_NCOMPS+4; cp++)
      cResult[cp] = state->deviceNTransfer[cp][(Guchar)(((alpha2 - aSrc) * cDest[cp] +
					      aSrc * pipe->cSrc[cp]) / alpha2)];
  }

  mask = 1;
  for (cp=0; cp < SPOT_NCOMPS+4; cp++) {
    if (state->overprintMask & mask) {
      pipe->destColorPtr[cp] = cResult[cp];
    }
    mask <<= 1;
  }
  pipe->destColorPtr += (SPOT_NCOMPS+4);
  *pipe->destAlphaPtr++ = aResult;

  ++pipe->x;
}
