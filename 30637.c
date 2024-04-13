GBool RGBGrayEncoder::fillBuf() {
  int c0, c1, c2;
  int i;

  if (eof) {
    return gFalse;
  }
  c0 = str->getChar();
  c1 = str->getChar();
  c2 = str->getChar();
  if (c2 == EOF) {
    eof = gTrue;
    return gFalse;
  }
  i = 255 - (3 * c0 + 6 * c1 + c2) / 10;
  if (i < 0) i = 0;
  bufPtr = bufEnd = buf;
  *bufEnd++ = (char) i;
  return gTrue;
}
