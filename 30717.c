void ASCII85Stream::reset() {
  str->reset();
  index = n = 0;
  eof = gFalse;
}
