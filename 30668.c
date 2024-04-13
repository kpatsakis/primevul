void FlateStream::getRawChars(int nChars, int *buffer) {
  for (int i = 0; i < nChars; ++i)
    buffer[i] = doGetRawChar();
}
