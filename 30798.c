void Splash::pipeRunSimpleDeviceN8(SplashPipe *pipe) {
  int mask = 1;
  for (int cp = 0; cp < SPOT_NCOMPS+4; cp++) {
    if (state->overprintMask & mask) {
      pipe->destColorPtr[cp] = state->deviceNTransfer[cp][pipe->cSrc[cp]];
    }
    mask <<=1;
  }
  pipe->destColorPtr += (SPOT_NCOMPS+4);
  *pipe->destAlphaPtr++ = 255;

  ++pipe->x;
}
