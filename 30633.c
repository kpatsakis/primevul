GBool ASCIIHexEncoder::fillBuf() {
  static const char *hex = "0123456789abcdef";
  int c;

  if (eof) {
    return gFalse;
  }
  bufPtr = bufEnd = buf;
  if ((c = str->getChar()) == EOF) {
    *bufEnd++ = '>';
    eof = gTrue;
  } else {
    if (lineLen >= 64) {
      *bufEnd++ = '\n';
      lineLen = 0;
    }
    *bufEnd++ = hex[(c >> 4) & 0x0f];
    *bufEnd++ = hex[c & 0x0f];
    lineLen += 2;
  }
  return gTrue;
}
