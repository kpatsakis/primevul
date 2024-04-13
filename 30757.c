MemStream::~MemStream() {
  if (needFree) {
    gfree(buf);
  }
}
