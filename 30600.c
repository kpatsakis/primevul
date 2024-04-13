CachedFileStream::CachedFileStream(CachedFile *ccA, Guint startA,
        GBool limitedA, Guint lengthA, Object *dictA)
  : BaseStream(dictA, lengthA)
{
  cc = ccA;
  start = startA;
  limited = limitedA;
  length = lengthA;
  bufPtr = bufEnd = buf;
  bufPos = start;
  savePos = 0;
  saved = gFalse;
}
