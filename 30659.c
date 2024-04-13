GooString *RunLengthStream::getPSFilter(int psLevel, const char *indent) {
  GooString *s;

  if (psLevel < 2) {
    return NULL;
  }
  if (!(s = str->getPSFilter(psLevel, indent))) {
    return NULL;
  }
  s->append(indent)->append("/RunLengthDecode filter\n");
  return s;
}
