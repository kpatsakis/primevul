MemStream::MemStream(char *bufA, Guint startA, Guint lengthA, Object *dictA):
    BaseStream(dictA, lengthA) {
  buf = bufA;
  start = startA;
  length = lengthA;
  bufEnd = buf + start + length;
  bufPtr = buf + start;
  needFree = gFalse;
}
