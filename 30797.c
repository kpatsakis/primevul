void Splash::pipeRunSimpleCMYK8(SplashPipe *pipe) {
  if (state->overprintMask & 1) {
    pipe->destColorPtr[0] = (state->overprintAdditive) ? 
              std::min<int>(pipe->destColorPtr[0] + state->cmykTransferC[pipe->cSrc[0]], 255) :
              state->cmykTransferC[pipe->cSrc[0]];
  }
  if (state->overprintMask & 2) {
    pipe->destColorPtr[1] = (state->overprintAdditive) ? 
              std::min<int>(pipe->destColorPtr[1] + state->cmykTransferM[pipe->cSrc[1]], 255) :
              state->cmykTransferM[pipe->cSrc[1]];
  }
  if (state->overprintMask & 4) {
    pipe->destColorPtr[2] = (state->overprintAdditive) ? 
              std::min<int>(pipe->destColorPtr[2] + state->cmykTransferY[pipe->cSrc[2]], 255) :
              state->cmykTransferY[pipe->cSrc[2]];
  }
  if (state->overprintMask & 8) {
    pipe->destColorPtr[3] = (state->overprintAdditive) ? 
              std::min<int>(pipe->destColorPtr[3] + state->cmykTransferK[pipe->cSrc[3]], 255) :
              state->cmykTransferK[pipe->cSrc[3]];
  }
  pipe->destColorPtr += 4;
  *pipe->destAlphaPtr++ = 255;

  ++pipe->x;
}
