Stream *MemStream::makeSubStream(Guint startA, GBool limited,
				 Guint lengthA, Object *dictA) {
  MemStream *subStr;
  Guint newLength;

  if (!limited || startA + lengthA > start + length) {
    newLength = start + length - startA;
  } else {
    newLength = lengthA;
  }
  subStr = new MemStream(buf, startA, newLength, dictA);
  return subStr;
}
