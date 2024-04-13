Stream *FileStream::makeSubStream(Guint startA, GBool limitedA,
				  Guint lengthA, Object *dictA) {
  return new FileStream(f, startA, limitedA, lengthA, dictA);
}
