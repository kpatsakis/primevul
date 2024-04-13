void Splash::pipeRunAAMono1(SplashPipe *pipe) {
  Guchar aSrc;
  SplashColor cDest;
  Guchar cResult0;

  cDest[0] = (*pipe->destColorPtr & pipe->destColorMask) ? 0xff : 0x00;

  aSrc = div255(pipe->aInput * pipe->shape);

  cResult0 = state->grayTransfer[(Guchar)div255((0xff - aSrc) * cDest[0] +
						aSrc * pipe->cSrc[0])];

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
