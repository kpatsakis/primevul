GBool LZWStream::processNextCode() {
  int code;
  int nextLength;
  int i, j;

  if (eof) {
    return gFalse;
  }

 start:
  code = getCode();
  if (code == EOF || code == 257) {
    eof = gTrue;
    return gFalse;
  }
  if (code == 256) {
    clearTable();
    goto start;
  }
  if (nextCode >= 4097) {
    error(errSyntaxError, getPos(),
	  "Bad LZW stream - expected clear-table code");
    clearTable();
  }

  nextLength = seqLength + 1;
  if (code < 256) {
    seqBuf[0] = code;
    seqLength = 1;
  } else if (code < nextCode) {
    seqLength = table[code].length;
    for (i = seqLength - 1, j = code; i > 0; --i) {
      seqBuf[i] = table[j].tail;
      j = table[j].head;
    }
    seqBuf[0] = j;
  } else if (code == nextCode) {
    seqBuf[seqLength] = newChar;
    ++seqLength;
  } else {
    error(errSyntaxError, getPos(), "Bad LZW stream - unexpected code");
    eof = gTrue;
    return gFalse;
  }
  newChar = seqBuf[0];
  if (first) {
    first = gFalse;
  } else {
    table[nextCode].length = nextLength;
    table[nextCode].head = prevCode;
    table[nextCode].tail = newChar;
    ++nextCode;
    if (nextCode + early == 512)
      nextBits = 10;
    else if (nextCode + early == 1024)
      nextBits = 11;
    else if (nextCode + early == 2048)
      nextBits = 12;
  }
  prevCode = code;

  seqIndex = 0;

  return gTrue;
}
