GBool RunLengthStream::fillBuf() {
  int c;
  int n, i;

  if (eof)
    return gFalse;
  c = str->getChar();
  if (c == 0x80 || c == EOF) {
    eof = gTrue;
    return gFalse;
  }
  if (c < 0x80) {
    n = c + 1;
    for (i = 0; i < n; ++i)
      buf[i] = (char)str->getChar();
  } else {
    n = 0x101 - c;
    c = str->getChar();
    for (i = 0; i < n; ++i)
      buf[i] = (char)c;
  }
  bufPtr = buf;
  bufEnd = buf + n;
  return gTrue;
}
