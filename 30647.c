int MemStream::getChars(int nChars, Guchar *buffer) {
  int n;

  if (nChars <= 0) {
    return 0;
  }
  if (bufEnd - bufPtr < nChars) {
    n = (int)(bufEnd - bufPtr);
  } else {
    n = nChars;
  }
  memcpy(buffer, bufPtr, n);
  bufPtr += n;
  return n;
}
