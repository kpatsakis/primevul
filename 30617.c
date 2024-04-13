inline void CCITTFaxStream::addPixelsNeg(int a1, int blackPixels) {
  if (a1 > codingLine[a0i]) {
    if (a1 > columns) {
      error(errSyntaxError, getPos(),
	    "CCITTFax row is wrong length ({0:d})", a1);
      err = gTrue;
      a1 = columns;
    }
    if ((a0i & 1) ^ blackPixels) {
      ++a0i;
    }
    codingLine[a0i] = a1;
  } else if (a1 < codingLine[a0i]) {
    if (a1 < 0) {
      error(errSyntaxError, getPos(), "Invalid CCITTFax code");
      err = gTrue;
      a1 = 0;
    }
    while (a0i > 0 && a1 <= codingLine[a0i - 1]) {
      --a0i;
    }
    codingLine[a0i] = a1;
  }
}
