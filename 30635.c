GBool RunLengthEncoder::fillBuf() {
  int c, c1, c2;
  int n;

  if (eof)
    return gFalse;

  if (nextEnd < bufEnd + 1) {
    if ((c1 = str->getChar()) == EOF) {
      eof = gTrue;
      return gFalse;
    }
  } else {
    c1 = bufEnd[0] & 0xff;
  }
  if (nextEnd < bufEnd + 2) {
    if ((c2 = str->getChar()) == EOF) {
      eof = gTrue;
      buf[0] = 0;
      buf[1] = c1;
      bufPtr = buf;
      bufEnd = &buf[2];
      return gTrue;
    }
  } else {
    c2 = bufEnd[1] & 0xff;
  }

  c = 0; // make gcc happy
  if (c1 == c2) {
    n = 2;
    while (n < 128 && (c = str->getChar()) == c1)
      ++n;
    buf[0] = (char)(257 - n);
    buf[1] = c1;
    bufEnd = &buf[2];
    if (c == EOF) {
      eof = gTrue;
    } else if (n < 128) {
      buf[2] = c;
      nextEnd = &buf[3];
    } else {
      nextEnd = bufEnd;
    }

  } else {
    buf[1] = c1;
    buf[2] = c2;
    n = 2;
    while (n < 128) {
      if ((c = str->getChar()) == EOF) {
	eof = gTrue;
	break;
      }
      ++n;
      buf[n] = c;
      if (buf[n] == buf[n-1])
	break;
    }
    if (buf[n] == buf[n-1]) {
      buf[0] = (char)(n-2-1);
      bufEnd = &buf[n-1];
      nextEnd = &buf[n+1];
    } else {
      buf[0] = (char)(n-1);
      bufEnd = nextEnd = &buf[n+1];
    }
  }
  bufPtr = buf;
  return gTrue;
}
