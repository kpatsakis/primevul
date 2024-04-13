LZWStream::~LZWStream() {
  if (pred) {
    delete pred;
  }
  delete str;
}
