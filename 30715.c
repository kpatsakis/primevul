void CachedFileStream::reset()
{
  savePos = (Guint)cc->tell();
  cc->seek(start, SEEK_SET);

  saved = gTrue;
  bufPtr = bufEnd = buf;
  bufPos = start;
}
