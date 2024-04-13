StreamPredictor::StreamPredictor(Stream *strA, int predictorA,
				 int widthA, int nCompsA, int nBitsA) {
  str = strA;
  predictor = predictorA;
  width = widthA;
  nComps = nCompsA;
  nBits = nBitsA;
  predLine = NULL;
  ok = gFalse;

  nVals = width * nComps;
  pixBytes = (nComps * nBits + 7) >> 3;
  rowBytes = ((nVals * nBits + 7) >> 3) + pixBytes;
  if (width <= 0 || nComps <= 0 || nBits <= 0 ||
      nComps > gfxColorMaxComps ||
      nBits > 16 ||
      width >= INT_MAX / nComps ||      // check for overflow in nVals
      nVals >= (INT_MAX - 7) / nBits) { // check for overflow in rowBytes
    return;
  }
  predLine = (Guchar *)gmalloc(rowBytes);
  memset(predLine, 0, rowBytes);
  predIdx = rowBytes;

  ok = gTrue;
}
