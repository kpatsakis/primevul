int FlateStream::getChars(int nChars, Guchar *buffer) {
  if (pred) {
    return pred->getChars(nChars, buffer);
  } else {
    for (int i = 0; i < nChars; ++i) {
      const int c = doGetRawChar();
      if (likely(c != EOF)) buffer[i] = c;
      else return i;
    }
    return nChars;
  }
}
