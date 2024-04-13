void ASCIIHexStream::reset() {
  str->reset();
  buf = EOF;
  eof = gFalse;
}
