GBool DCTStream::readBaselineSOF() {
  int length;
  int prec;
  int i;
  int c;

  length = read16();
  prec = str->getChar();
  height = read16();
  width = read16();
  numComps = str->getChar();
  if (numComps <= 0 || numComps > 4) {
    error(errSyntaxError, getPos(), "Bad number of components in DCT stream");
    numComps = 0;
    return gFalse;
  }
  if (prec != 8) {
    error(errSyntaxError, getPos(), "Bad DCT precision {0:d}", prec);
    return gFalse;
  }
  for (i = 0; i < numComps; ++i) {
    compInfo[i].id = str->getChar();
    c = str->getChar();
    compInfo[i].hSample = (c >> 4) & 0x0f;
    compInfo[i].vSample = c & 0x0f;
    compInfo[i].quantTable = str->getChar();
    if (compInfo[i].hSample < 1 || compInfo[i].hSample > 4 ||
	compInfo[i].vSample < 1 || compInfo[i].vSample > 4) {
      error(errSyntaxError, getPos(), "Bad DCT sampling factor");
      return gFalse;
    }
    if (compInfo[i].quantTable < 0 || compInfo[i].quantTable > 3) {
      error(errSyntaxError, getPos(), "Bad DCT quant table selector");
      return gFalse;
    }
  }
  progressive = gFalse;
  return gTrue;
}
