int DCTStream::readMarker() {
  int c;

  do {
    do {
      c = str->getChar();
    } while (c != 0xff && c != EOF);
    while (c == 0xff) {
      c = str->getChar();
    }
  } while (c == 0x00);
  return c;
}
