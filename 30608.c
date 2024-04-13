ImageStream::ImageStream(Stream *strA, int widthA, int nCompsA, int nBitsA) {
  int imgLineSize;

  str = strA;
  width = widthA;
  nComps = nCompsA;
  nBits = nBitsA;

  nVals = width * nComps;
  inputLineSize = (nVals * nBits + 7) >> 3;
  if (nBits <= 0 || nVals > INT_MAX / nBits - 7) {
    inputLineSize = -1;
  }
  inputLine = (Guchar *)gmallocn_checkoverflow(inputLineSize, sizeof(char));
  if (nBits == 8) {
    imgLine = (Guchar *)inputLine;
  } else {
    if (nBits == 1) {
      imgLineSize = (nVals + 7) & ~7;
    } else {
      imgLineSize = nVals;
    }
    if (width > INT_MAX / nComps) {
      imgLineSize = -1;
    }
    imgLine = (Guchar *)gmallocn(imgLineSize, sizeof(Guchar));
  }
  imgIdx = nVals;
}
