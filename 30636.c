GBool CMYKGrayEncoder::fillBuf() {
  int c0, c1, c2, c3;
  int i;

  if (eof) {
    return gFalse;
  }
  c0 = str->getChar();
  c1 = str->getChar();
  c2 = str->getChar();
  c3 = str->getChar();
  if (c3 == EOF) {
    eof = gTrue;
    return gFalse;
  }
  i = (3 * c0 + 6 * c1 + c2) / 10 + c3;
  if (i > 255) i = 255;
  bufPtr = bufEnd = buf;
  *bufEnd++ = (char) i;
  return gTrue;
}
