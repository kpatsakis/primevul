FixedLengthEncoder::FixedLengthEncoder(Stream *strA, int lengthA):
    FilterStream(strA) {
  length = lengthA;
  count = 0;
}
