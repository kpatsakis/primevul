void Splash::pipeRunAACMYK8(SplashPipe *pipe) {
  Guchar aSrc, aDest, alpha2, aResult;
  SplashColor cDest;
  Guchar cResult0, cResult1, cResult2, cResult3;

  cDest[0] = pipe->destColorPtr[0];
  cDest[1] = pipe->destColorPtr[1];
  cDest[2] = pipe->destColorPtr[2];
  cDest[3] = pipe->destColorPtr[3];
  aDest = *pipe->destAlphaPtr;

  aSrc = div255(pipe->aInput * pipe->shape);

  aResult = aSrc + aDest - div255(aSrc * aDest);
  alpha2 = aResult;

  if (alpha2 == 0) {
    cResult0 = 0;
    cResult1 = 0;
    cResult2 = 0;
    cResult3 = 0;
  } else {
    cResult0 = state->cmykTransferC[(Guchar)(((alpha2 - aSrc) * cDest[0] +
					      aSrc * pipe->cSrc[0]) / alpha2)];
    cResult1 = state->cmykTransferM[(Guchar)(((alpha2 - aSrc) * cDest[1] +
					      aSrc * pipe->cSrc[1]) / alpha2)];
    cResult2 = state->cmykTransferY[(Guchar)(((alpha2 - aSrc) * cDest[2] +
					      aSrc * pipe->cSrc[2]) / alpha2)];
    cResult3 = state->cmykTransferK[(Guchar)(((alpha2 - aSrc) * cDest[3] +
					      aSrc * pipe->cSrc[3]) / alpha2)];
  }

  if (state->overprintMask & 1) {
    pipe->destColorPtr[0] = (state->overprintAdditive && pipe->shape != 0) ? 
              std::min<int>(pipe->destColorPtr[0] + cResult0, 255) :
              cResult0;
  }
  if (state->overprintMask & 2) {
    pipe->destColorPtr[1] = (state->overprintAdditive && pipe->shape != 0) ? 
              std::min<int>(pipe->destColorPtr[1] + cResult1, 255) :
              cResult1;
  }
  if (state->overprintMask & 4) {
    pipe->destColorPtr[2] = (state->overprintAdditive && pipe->shape != 0) ? 
              std::min<int>(pipe->destColorPtr[2] + cResult2, 255) :
              cResult2;
  }
  if (state->overprintMask & 8) {
    pipe->destColorPtr[3] = (state->overprintAdditive && pipe->shape != 0) ? 
              std::min<int>(pipe->destColorPtr[3] + cResult3, 255) :
              cResult3;
  }
  pipe->destColorPtr += 4;
  *pipe->destAlphaPtr++ = aResult;

  ++pipe->x;
}
