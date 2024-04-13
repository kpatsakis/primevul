ASCIIHexEncoder::ASCIIHexEncoder(Stream *strA):
    FilterStream(strA) {
  bufPtr = bufEnd = buf;
  lineLen = 0;
  eof = gFalse;
}
