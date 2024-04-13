void BufStream::reset() {
  int i;

  str->reset();
  for (i = 0; i < bufSize; ++i) {
    buf[i] = str->getChar();
  }
}
