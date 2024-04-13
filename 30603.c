EmbedStream::EmbedStream(Stream *strA, Object *dictA,
			 GBool limitedA, Guint lengthA):
    BaseStream(dictA, lengthA) {
  str = strA;
  limited = limitedA;
  length = lengthA;
}
