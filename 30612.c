RunLengthEncoder::RunLengthEncoder(Stream *strA):
    FilterStream(strA) {
  bufPtr = bufEnd = nextEnd = buf;
  eof = gFalse;
}
