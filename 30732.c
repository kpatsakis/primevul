void CachedFileStream::setPos(Guint pos, int dir)
{
  Guint size;

  if (dir >= 0) {
    cc->seek(pos, SEEK_SET);
    bufPos = pos;
  } else {
    cc->seek(0, SEEK_END);
    size = (Guint)cc->tell();

    if (pos > size)
      pos = (Guint)size;

    cc->seek(-(int)pos, SEEK_END);
    bufPos = (Guint)cc->tell();
  }

  bufPtr = bufEnd = buf;
}
