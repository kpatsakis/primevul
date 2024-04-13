int EmbedStream::getChar() {
  if (limited && !length) {
    return EOF;
  }
  --length;
  return str->getChar();
}
