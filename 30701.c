int DCTStream::readBit() {
  int bit;
  int c, c2;

  if (inputBits == 0) {
    if ((c = str->getChar()) == EOF)
      return EOF;
    if (c == 0xff) {
      do {
	c2 = str->getChar();
      } while (c2 == 0xff);
      if (c2 != 0x00) {
	error(errSyntaxError, getPos(), "Bad DCT data: missing 00 after ff");
	return EOF;
      }
    }
    inputBuf = c;
    inputBits = 8;
  }
  bit = (inputBuf >> (inputBits - 1)) & 1;
  --inputBits;
  return bit;
}
