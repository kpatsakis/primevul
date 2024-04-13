int LZWStream::getChar() {
  if (pred) {
    return pred->getChar();
  }
  if (eof) {
    return EOF;
  }
  if (seqIndex >= seqLength) {
    if (!processNextCode()) {
      return EOF;
    }
  }
  return seqBuf[seqIndex++];
}
