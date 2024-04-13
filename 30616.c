inline void CCITTFaxStream::addPixels(int a1, int blackPixels) {
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
  }
}
