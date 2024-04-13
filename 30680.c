int StreamPredictor::lookChar() {
  if (predIdx >= rowBytes) {
    if (!getNextLine()) {
      return EOF;
    }
  }
  return predLine[predIdx];
}
