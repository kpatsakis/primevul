void ASCII85Encoder::reset() {
  str->reset();
  bufPtr = bufEnd = buf;
  lineLen = 0;
  eof = gFalse;
}
