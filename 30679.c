short CCITTFaxStream::lookBits(int n) {
  int c;

  while (inputBits < n) {
    if ((c = str->getChar()) == EOF) {
      if (inputBits == 0) {
	return EOF;
      }
      return (inputBuf << (n - inputBits)) & (0xffffffff >> (32 - n));
    }
    inputBuf = (inputBuf << 8) + c;
    inputBits += 8;
  }
  return (inputBuf >> (inputBits - n)) & (0xffffffff >> (32 - n));
}
