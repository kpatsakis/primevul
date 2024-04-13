int JBIG2Stream::getChars(int nChars, Guchar *buffer) {
  int n, i;

  if (nChars <= 0) {
    return 0;
  }
  if (dataEnd - dataPtr < nChars) {
    n = (int)(dataEnd - dataPtr);
  } else {
    n = nChars;
  }
  for (i = 0; i < n; ++i) {
    buffer[i] = *dataPtr++ ^ 0xff;
  }
  return n;
}
