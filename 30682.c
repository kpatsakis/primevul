int FlateStream::lookChar() {
  int c;

  if (pred) {
    return pred->lookChar();
  }
  while (remain == 0) {
    if (endOfBlock && eof)
      return EOF;
    readSome();
  }
  c = buf[index];
  return c;
}
