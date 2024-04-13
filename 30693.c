void MemStream::moveStart(int delta) {
  start += delta;
  length -= delta;
  bufPtr = buf + start;
}
