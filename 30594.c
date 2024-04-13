ASCII85Stream::ASCII85Stream(Stream *strA):
    FilterStream(strA) {
  index = n = 0;
  eof = gFalse;
}
