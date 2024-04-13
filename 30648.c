int EmbedStream::getChars(int nChars, Guchar *buffer) {
  if (nChars <= 0) {
    return 0;
  }
  if (limited && length < (Guint)nChars) {
    nChars = (int)length;
  }
  return str->doGetChars(nChars, buffer);
}
