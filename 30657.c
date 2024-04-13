GooString *ASCII85Stream::getPSFilter(int psLevel, const char *indent) {
  GooString *s;

  if (psLevel < 2) {
    return NULL;
  }
  if (!(s = str->getPSFilter(psLevel, indent))) {
    return NULL;
  }
  s->append(indent)->append("/ASCII85Decode filter\n");
  return s;
}
