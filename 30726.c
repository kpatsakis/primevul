void RunLengthEncoder::reset() {
  str->reset();
  bufPtr = bufEnd = nextEnd = buf;
  eof = gFalse;
}
