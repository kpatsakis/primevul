GBool DCTStream::readQuantTables() {
  int length, prec, i, index;

  length = read16() - 2;
  while (length > 0) {
    index = str->getChar();
    prec = (index >> 4) & 0x0f;
    index &= 0x0f;
    if (prec > 1 || index >= 4) {
      error(errSyntaxError, getPos(), "Bad DCT quantization table");
      return gFalse;
    }
    if (index == numQuantTables) {
      numQuantTables = index + 1;
    }
    for (i = 0; i < 64; ++i) {
      if (prec) {
	quantTables[index][dctZigZag[i]] = read16();
      } else {
	quantTables[index][dctZigZag[i]] = str->getChar();
      }
    }
    if (prec) {
      length -= 129;
    } else {
      length -= 65;
    }
  }
  return gTrue;
}
