DCTStream::~DCTStream() {
  close();
  delete str;
}
