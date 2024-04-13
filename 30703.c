GBool DCTStream::readHuffmanTables() {
  DCTHuffTable *tbl;
  int length;
  int index;
  Gushort code;
  Guchar sym;
  int i;
  int c;

  length = read16() - 2;
  while (length > 0) {
    index = str->getChar();
    --length;
    if ((index & 0x0f) >= 4) {
      error(errSyntaxError, getPos(), "Bad DCT Huffman table");
      return gFalse;
    }
    if (index & 0x10) {
      index &= 0x0f;
      if (index >= numACHuffTables)
	numACHuffTables = index+1;
      tbl = &acHuffTables[index];
    } else {
      index &= 0x0f;
      if (index >= numDCHuffTables)
	numDCHuffTables = index+1;
      tbl = &dcHuffTables[index];
    }
    sym = 0;
    code = 0;
    for (i = 1; i <= 16; ++i) {
      c = str->getChar();
      tbl->firstSym[i] = sym;
      tbl->firstCode[i] = code;
      tbl->numCodes[i] = c;
      sym += c;
      code = (code + c) << 1;
    }
    length -= 16;
    for (i = 0; i < sym; ++i)
      tbl->sym[i] = str->getChar();
    length -= sym;
  }
  return gTrue;
}
