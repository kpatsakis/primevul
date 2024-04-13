int RunLengthStream::getChars(int nChars, Guchar *buffer) {
  int n, m;

  n = 0;
  while (n < nChars) {
    if (bufPtr >= bufEnd) {
      if (!fillBuf()) {
	break;
      }
    }
    m = (int)(bufEnd - bufPtr);
    if (m > nChars - n) {
      m = nChars - n;
    }
    memcpy(buffer + n, bufPtr, m);
    bufPtr += m;
    n += m;
  }
  return n;
}
