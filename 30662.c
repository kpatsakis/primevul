GooString *FlateStream::getPSFilter(int psLevel, const char *indent) {
  GooString *s;

  if (psLevel < 3 || pred) {
    return NULL;
  }
  if (!(s = str->getPSFilter(psLevel, indent))) {
    return NULL;
  }
  s->append(indent)->append("<< >> /FlateDecode filter\n");
  return s;
}
