int LZWStream::getCode() {
  int c;
  int code;

  while (inputBits < nextBits) {
    if ((c = str->getChar()) == EOF)
      return EOF;
    inputBuf = (inputBuf << 8) | (c & 0xff);
    inputBits += 8;
  }
  code = (inputBuf >> (inputBits - nextBits)) & ((1 << nextBits) - 1);
  inputBits -= nextBits;
  return code;
}
