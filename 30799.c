void Splash::pipeRunSimpleMono1(SplashPipe *pipe) {
  Guchar cResult0;

  cResult0 = state->grayTransfer[pipe->cSrc[0]];
  if (state->screen->test(pipe->x, pipe->y, cResult0)) {
    *pipe->destColorPtr |= pipe->destColorMask;
  } else {
    *pipe->destColorPtr &= ~pipe->destColorMask;
  }
  if (!(pipe->destColorMask >>= 1)) {
    pipe->destColorMask = 0x80;
    ++pipe->destColorPtr;
  }

  ++pipe->x;
}
