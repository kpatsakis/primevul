void JBIG2Stream::readExtensionSeg(Guint length) {
  Guint i;

  for (i = 0; i < length; ++i) {
    if (curStr->getChar() == EOF) {
      break;
    }
  }
}
