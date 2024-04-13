GooString *CCITTFaxStream::getPSFilter(int psLevel, const char *indent) {
  GooString *s;
  char s1[50];

  if (psLevel < 2) {
    return NULL;
  }
  if (!(s = str->getPSFilter(psLevel, indent))) {
    return NULL;
  }
  s->append(indent)->append("<< ");
  if (encoding != 0) {
    sprintf(s1, "/K %d ", encoding);
    s->append(s1);
  }
  if (endOfLine) {
    s->append("/EndOfLine true ");
  }
  if (byteAlign) {
    s->append("/EncodedByteAlign true ");
  }
  sprintf(s1, "/Columns %d ", columns);
  s->append(s1);
  if (rows != 0) {
    sprintf(s1, "/Rows %d ", rows);
    s->append(s1);
  }
  if (!endOfBlock) {
    s->append("/EndOfBlock false ");
  }
  if (black) {
    s->append("/BlackIs1 true ");
  }
  s->append(">> /CCITTFaxDecode filter\n");
  return s;
}
