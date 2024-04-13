RGBGrayEncoder::RGBGrayEncoder(Stream *strA):
    FilterStream(strA) {
  bufPtr = bufEnd = buf;
  eof = gFalse;
}
