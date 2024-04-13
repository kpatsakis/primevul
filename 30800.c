void Splash::pipeRunSimpleMono8(SplashPipe *pipe) {
  *pipe->destColorPtr++ = state->grayTransfer[pipe->cSrc[0]];
  *pipe->destAlphaPtr++ = 255;

  ++pipe->x;
}
