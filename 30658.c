GooString *LZWStream::getPSFilter(int psLevel, const char *indent) {
  GooString *s;

  if (psLevel < 2 || pred) {
    return NULL;
  }
  if (!(s = str->getPSFilter(psLevel, indent))) {
    return NULL;
  }
  s->append(indent)->append("<< ");
  if (!early) {
    s->append("/EarlyChange 0 ");
  }
  s->append(">> /LZWDecode filter\n");
  return s;
}
