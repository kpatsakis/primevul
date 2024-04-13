GBool ImageStream::getPixel(Guchar *pix) {
  int i;

  if (imgIdx >= nVals) {
    if (!getLine()) {
      return gFalse;
    }
    imgIdx = 0;
  }
  for (i = 0; i < nComps; ++i) {
    pix[i] = imgLine[imgIdx++];
  }
  return gTrue;
}
