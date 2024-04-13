BufStream::~BufStream() {
  gfree(buf);
  delete str;
}
