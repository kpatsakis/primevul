GBool DCTStream::readRestartInterval() {
  int length;

  length = read16();
  if (length != 4) {
    error(errSyntaxError, getPos(), "Bad DCT restart interval");
    return gFalse;
  }
  restartInterval = read16();
  return gTrue;
}
