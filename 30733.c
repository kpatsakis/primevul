void MemStream::setPos(Guint pos, int dir) {
  Guint i;

  if (dir >= 0) {
    i = pos;
  } else {
    i = start + length - pos;
  }
  if (i < start) {
    i = start;
  } else if (i > start + length) {
    i = start + length;
  }
  bufPtr = buf + i;
}
