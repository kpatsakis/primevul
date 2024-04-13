void Splash::pipeRunSimpleXBGR8(SplashPipe *pipe) {
  *pipe->destColorPtr++ = state->rgbTransferB[pipe->cSrc[2]];
  *pipe->destColorPtr++ = state->rgbTransferG[pipe->cSrc[1]];
  *pipe->destColorPtr++ = state->rgbTransferR[pipe->cSrc[0]];
  *pipe->destColorPtr++ = 255;
  *pipe->destAlphaPtr++ = 255;

  ++pipe->x;
}
