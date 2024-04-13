FlateStream::FlateStream(Stream *strA, int predictor, int columns,
			 int colors, int bits):
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
  litCodeTab.codes = NULL;
  distCodeTab.codes = NULL;
  memset(buf, 0, flateWindow);
}
