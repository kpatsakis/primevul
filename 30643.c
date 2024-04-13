int FlateStream::getChar() {
  if (pred) {
    return pred->getChar();
  }
  return doGetRawChar();
}
