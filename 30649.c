int LZWStream::getChars(int nChars, Guchar *buffer) {
  int n, m;

  if (pred) {
    return pred->getChars(nChars, buffer);
  }
  if (eof) {
    return 0;
  }
  n = 0;
  while (n < nChars) {
    if (seqIndex >= seqLength) {
      if (!processNextCode()) {
	break;
      }
    }
    m = seqLength - seqIndex;
    if (m > nChars - n) {
      m = nChars - n;
    }
    memcpy(buffer + n, seqBuf + seqIndex, m);
    seqIndex += m;
    n += m;
  }
  return n;
}
