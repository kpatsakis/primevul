void LZWStream::clearTable() {
  nextCode = 258;
  nextBits = 9;
  seqIndex = seqLength = 0;
  first = gTrue;
}
