void CachedFileStream::close()
{
  if (saved) {
    cc->seek(savePos, SEEK_SET);
    saved = gFalse;
  }
}
