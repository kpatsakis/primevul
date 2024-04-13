short CCITTFaxStream::getBlackCode() {
  short code;
  const CCITTCode *p;
  int n;

  code = 0; // make gcc happy
  if (endOfBlock) {
    code = lookBits(13);
    if (code == EOF) {
      return 1;
    }
    if ((code >> 7) == 0) {
      p = &blackTab1[code];
    } else if ((code >> 9) == 0 && (code >> 7) != 0) {
      p = &blackTab2[(code >> 1) - 64];
    } else {
      p = &blackTab3[code >> 7];
    }
    if (p->bits > 0) {
      eatBits(p->bits);
      return p->n;
    }
  } else {
    for (n = 2; n <= 6; ++n) {
      code = lookBits(n);
      if (code == EOF) {
	return 1;
      }
      if (n < 6) {
	code <<= 6 - n;
      }
      p = &blackTab3[code];
      if (p->bits == n) {
	eatBits(n);
	return p->n;
      }
    }
    for (n = 7; n <= 12; ++n) {
      code = lookBits(n);
      if (code == EOF) {
	return 1;
      }
      if (n < 12) {
	code <<= 12 - n;
      }
      if (code >= 64) {
	p = &blackTab2[code - 64];
	if (p->bits == n) {
	  eatBits(n);
	  return p->n;
	}
      }
    }
    for (n = 10; n <= 13; ++n) {
      code = lookBits(n);
      if (code == EOF) {
	return 1;
      }
      if (n < 13) {
	code <<= 13 - n;
      }
      p = &blackTab1[code];
      if (p->bits == n) {
	eatBits(n);
	return p->n;
      }
    }
  }
  error(errSyntaxError, getPos(),
	"Bad black code ({0:04x}) in CCITTFax stream", code);
  eatBits(1);
  return 1;
}
