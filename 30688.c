Stream *CachedFileStream::makeSubStream(Guint startA, GBool limitedA,
        Guint lengthA, Object *dictA)
{
  cc->incRefCnt();
  return new CachedFileStream(cc, startA, limitedA, lengthA, dictA);
}
