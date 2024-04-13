void CMYKGrayEncoder::reset() {
  str->reset();
  bufPtr = bufEnd = buf;
  eof = gFalse;
}
