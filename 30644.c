int BufStream::getChar() {
  int c, i;

  c = buf[0];
  for (i = 1; i < bufSize; ++i) {
    buf[i-1] = buf[i];
  }
  buf[bufSize - 1] = str->getChar();
  return c;
}
