GBool CachedFileStream::fillBuf()
{
  int n;

  bufPos += bufEnd - buf;
  bufPtr = bufEnd = buf;
  if (limited && bufPos >= start + length) {
    return gFalse;
  }
  if (limited && bufPos + cachedStreamBufSize > start + length) {
    n = start + length - bufPos;
  } else {
    n = cachedStreamBufSize - (bufPos % cachedStreamBufSize);
  }
  cc->read(buf, 1, n);
  bufEnd = buf + n;
  if (bufPtr >= bufEnd) {
    return gFalse;
  }
  return gTrue;
}
