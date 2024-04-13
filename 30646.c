int StreamPredictor::getChars(int nChars, Guchar *buffer) {
  int n, m;

  n = 0;
  while (n < nChars) {
    if (predIdx >= rowBytes) {
      if (!getNextLine()) {
	break;
      }
    }
    m = rowBytes - predIdx;
    if (m > nChars - n) {
      m = nChars - n;
    }
    memcpy(buffer + n, predLine + predIdx, m);
    predIdx += m;
    n += m;
  }
  return n;
}
