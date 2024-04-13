GBool DCTStream::readAdobeMarker() {
  int length, i;
  char buf[12];
  int c;

  length = read16();
  if (length < 14) {
    goto err;
  }
  for (i = 0; i < 12; ++i) {
    if ((c = str->getChar()) == EOF) {
      goto err;
    }
    buf[i] = c;
  }
  if (strncmp(buf, "Adobe", 5)) {
    goto err;
  }
  colorXform = buf[11];
  gotAdobeMarker = gTrue;
  for (i = 14; i < length; ++i) {
    if (str->getChar() == EOF) {
      goto err;
    }
  }
  return gTrue;

 err:
  error(errSyntaxError, getPos(), "Bad DCT Adobe APP14 marker");
  return gFalse;
}
