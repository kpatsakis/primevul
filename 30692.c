void CachedFileStream::moveStart(int delta)
{
  start += delta;
  bufPtr = bufEnd = buf;
  bufPos = start;
}
