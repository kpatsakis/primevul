void ASCIIHexEncoder::reset() {
  str->reset();
  bufPtr = bufEnd = buf;
  lineLen = 0;
  eof = gFalse;
}
