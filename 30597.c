BufStream::BufStream(Stream *strA, int bufSizeA): FilterStream(strA) {
  bufSize = bufSizeA;
  buf = (int *)gmallocn(bufSize, sizeof(int));
}
