short CCITTFaxStream::getWhiteCode() {
  short code;
  const CCITTCode *p;
  int n;

  code = 0; // make gcc happy
  if (endOfBlock) {
    code = lookBits(12);
    if (code == EOF) {
      return 1;
    }
    if ((code >> 5) == 0) {
      p = &whiteTab1[code];
    } else {
      p = &whiteTab2[code >> 3];
    }
    if (p->bits > 0) {
      eatBits(p->bits);
      return p->n;
    }
  } else {
    for (n = 1; n <= 9; ++n) {
      code = lookBits(n);
      if (code == EOF) {
	return 1;
      }
      if (n < 9) {
	code <<= 9 - n;
      }
      p = &whiteTab2[code];
      if (p->bits == n) {
	eatBits(n);
	return p->n;
      }
    }
    for (n = 11; n <= 12; ++n) {
      code = lookBits(n);
      if (code == EOF) {
	return 1;
      }
      if (n < 12) {
	code <<= 12 - n;
      }
      p = &whiteTab1[code];
      if (p->bits == n) {
	eatBits(n);
	return p->n;
      }
    }
  }
  error(errSyntaxError, getPos(),
	"Bad white code ({0:04x}) in CCITTFax stream", code);
  eatBits(1);
  return 1;
}
