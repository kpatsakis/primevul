void RGBGrayEncoder::reset() {
  str->reset();
  bufPtr = bufEnd = buf;
  eof = gFalse;
}
