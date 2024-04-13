void LZWStream::reset() {
  str->reset();
  eof = gFalse;
  inputBits = 0;
  clearTable();
}
