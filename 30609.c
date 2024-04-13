LZWStream::LZWStream(Stream *strA, int predictor, int columns, int colors,
		     int bits, int earlyA):
    FilterStream(strA) {
  if (predictor != 1) {
    pred = new StreamPredictor(this, predictor, columns, colors, bits);
    if (!pred->isOk()) {
      delete pred;
      pred = NULL;
    }
  } else {
    pred = NULL;
  }
  early = earlyA;
  eof = gFalse;
  inputBits = 0;
  clearTable();
}
