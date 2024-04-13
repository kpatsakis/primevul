int FixedLengthEncoder::getChar() {
  if (length >= 0 && count >= length)
    return EOF;
  ++count;
  return str->getChar();
}
