GBool ASCII85Encoder::fillBuf() {
  Guint t;
  char buf1[5];
  int c0, c1, c2, c3;
  int n, i;

  if (eof) {
    return gFalse;
  }
  c0 = str->getChar();
  c1 = str->getChar();
  c2 = str->getChar();
  c3 = str->getChar();
  bufPtr = bufEnd = buf;
  if (c3 == EOF) {
    if (c0 == EOF) {
      n = 0;
      t = 0;
    } else {
      if (c1 == EOF) {
	n = 1;
	t = c0 << 24;
      } else if (c2 == EOF) {
	n = 2;
	t = (c0 << 24) | (c1 << 16);
      } else {
	n = 3;
	t = (c0 << 24) | (c1 << 16) | (c2 << 8);
      }
      for (i = 4; i >= 0; --i) {
	buf1[i] = (char)(t % 85 + 0x21);
	t /= 85;
      }
      for (i = 0; i <= n; ++i) {
	*bufEnd++ = buf1[i];
	if (++lineLen == 65) {
	  *bufEnd++ = '\n';
	  lineLen = 0;
	}
      }
    }
    *bufEnd++ = '~';
    *bufEnd++ = '>';
    eof = gTrue;
  } else {
    t = (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
    if (t == 0) {
      *bufEnd++ = 'z';
      if (++lineLen == 65) {
	*bufEnd++ = '\n';
	lineLen = 0;
      }
    } else {
      for (i = 4; i >= 0; --i) {
	buf1[i] = (char)(t % 85 + 0x21);
	t /= 85;
      }
      for (i = 0; i <= 4; ++i) {
	*bufEnd++ = buf1[i];
	if (++lineLen == 65) {
	  *bufEnd++ = '\n';
	  lineLen = 0;
	}
      }
    }
  }
  return gTrue;
}
