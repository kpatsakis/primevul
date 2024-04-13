int ASCII85Stream::lookChar() {
  int k;
  Gulong t;

  if (index >= n) {
    if (eof)
      return EOF;
    index = 0;
    do {
      c[0] = str->getChar();
    } while (Lexer::isSpace(c[0]));
    if (c[0] == '~' || c[0] == EOF) {
      eof = gTrue;
      n = 0;
      return EOF;
    } else if (c[0] == 'z') {
      b[0] = b[1] = b[2] = b[3] = 0;
      n = 4;
    } else {
      for (k = 1; k < 5; ++k) {
	do {
	  c[k] = str->getChar();
	} while (Lexer::isSpace(c[k]));
	if (c[k] == '~' || c[k] == EOF)
	  break;
      }
      n = k - 1;
      if (k < 5 && (c[k] == '~' || c[k] == EOF)) {
	for (++k; k < 5; ++k)
	  c[k] = 0x21 + 84;
	eof = gTrue;
      }
      t = 0;
      for (k = 0; k < 5; ++k)
	t = t * 85 + (c[k] - 0x21);
      for (k = 3; k >= 0; --k) {
	b[k] = (int)(t & 0xff);
	t >>= 8;
      }
    }
  }
  return b[index];
}
