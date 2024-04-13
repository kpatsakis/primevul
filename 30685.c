int FixedLengthEncoder::lookChar() {
  if (length >= 0 && count >= length)
    return EOF;
  return str->getChar();
}
